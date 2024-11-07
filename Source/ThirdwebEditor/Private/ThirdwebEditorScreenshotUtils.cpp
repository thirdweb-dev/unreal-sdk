// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "ThirdwebEditorScreenshotUtils.h"

#include "ImageUtils.h"
#include "ThirdwebEditorCommon.h"
#include "ThirdwebEditorLog.h"
#include "ThirdwebEditorSettings.h"
#include "Algo/RemoveIf.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Framework/Notifications/NotificationManager.h"
#include "HAL/PlatformApplicationMisc.h"
#include "Kismet/KismetStringLibrary.h"
#include "Slate/WidgetRenderer.h"
#include "Widgets/Notifications/SNotificationList.h"

#define LOCTEXT_NAMESPACE "FThirdwebEditorModule"

void FThirdwebEditorScreenshotUtils::TakeScreenshot()
{
	if (TSet<TSharedPtr<SGraphEditor>> GraphEditors = FindGraphEditors(); !GraphEditors.IsEmpty())
	{
		TArray<FString> Paths;

		const bool bHasSelectedNodes = HasSelectedNodes(GraphEditors);
		for (const TSharedPtr<SGraphEditor> GraphEditor : GraphEditors)
		{
			if (bHasSelectedNodes && HasSelectedNodes(GraphEditor))
			{
				if (const FString Path = SaveScreenshot(CaptureGraphEditor(GraphEditor)); !Path.IsEmpty())
				{
					Paths.Add(Path);
				}
			}
		}

		if (!Paths.IsEmpty() && UThirdwebEditorSettings::NotificationsEnabled())
		{
			ShowNotification(Paths);
		}
	} else TWE_LOG(Error, TEXT("FThirdwebEditorScreenshotUtils::TakeScreenshot::No Graph Editors"))
}

FString FThirdwebEditorScreenshotUtils::SaveScreenshot(const FThirdwebEditorScreenshotData& InData)
{
	if (InData.IsValid())
	{
		const FString ScreenshotDir = UThirdwebEditorSettings::GetScreenshotSaveDirectory().Path;
		const FString BaseName = UThirdwebEditorSettings::HasCustomScreenshotBaseName() || InData.CustomName.IsEmpty() ? UThirdwebEditorSettings::GetCustomScreenshotBaseName() : InData.CustomName;
		const auto FileExtension = GetExtension(UThirdwebEditorSettings::GetScreenshotFormat());
		const auto Path = FPaths::Combine(ScreenshotDir, BaseName);

		FString Filename;
		FFileHelper::GenerateNextBitmapFilename(Path, FileExtension, Filename);

		if (FImageUtils::SaveImageByExtension(*Filename, FImageView(InData.ColorData.GetData(), InData.Size.X, InData.Size.Y), UThirdwebEditorSettings::GetScreenshotQuality()))
		{
			return Filename;
		}
	}

	return FString();
}

FThirdwebEditorScreenshotData FThirdwebEditorScreenshotUtils::CaptureGraphEditor(const TSharedPtr<SGraphEditor>& InGraphEditor)
{
	if (InGraphEditor.IsValid())
	{
		FVector2D CachedViewLocation, NewViewLocation, WindowSize;
		float CachedZoomAmount, NewZoomAmount, WindowSizeScale = 1.f;

		const auto SelectedNodes = InGraphEditor->GetSelectedNodes();

		InGraphEditor->GetViewLocation(CachedViewLocation, CachedZoomAmount);

		if (SelectedNodes.Num() > 0)
		{
			FSlateRect BoundsForSelectedNodes;
			InGraphEditor->GetBoundsForSelectedNodes(BoundsForSelectedNodes, UThirdwebEditorSettings::GetScreenshotPadding());

			NewViewLocation = BoundsForSelectedNodes.GetTopLeft();
			NewZoomAmount = WindowSizeScale = UThirdwebEditorSettings::GetScreenshotZoom();
			WindowSize = BoundsForSelectedNodes.GetSize();
		}
		else
		{
			NewViewLocation = CachedViewLocation;
			NewZoomAmount = CachedZoomAmount;
			WindowSize = InGraphEditor->GetTickSpaceGeometry().GetLocalSize() * FPlatformApplicationMisc::GetDPIScaleFactorAtPoint(0.0f, 0.0f);
			InGraphEditor->SetViewLocation(NewViewLocation, CachedZoomAmount);
		}

		InGraphEditor->SetViewLocation(NewViewLocation, NewZoomAmount);
		FIntPoint SizeConstraints = UThirdwebEditorSettings::GetScreenshotSizeConstraints();
		WindowSize = WindowSize.ClampAxes(SizeConstraints.X, SizeConstraints.Y);
		WindowSize *= WindowSizeScale;

		InGraphEditor->ClearSelectionSet();

		const TStrongObjectPtr RenderTarget = TStrongObjectPtr(DrawGraphEditor(InGraphEditor, WindowSize));

		FThirdwebEditorScreenshotData ScreenshotData(WindowSize);
		RenderTarget->GameThread_GetRenderTargetResource()->ReadPixels(ScreenshotData.ColorData);

		RestoreNodeSelection(InGraphEditor, SelectedNodes);
		InGraphEditor->SetViewLocation(CachedViewLocation, CachedZoomAmount);

		if (!UThirdwebEditorSettings::HasCustomScreenshotBaseName())
		{
			ScreenshotData.CustomName = GenerateName(InGraphEditor);
		}

		return ScreenshotData;
	}

	return FThirdwebEditorScreenshotData();
}

void FThirdwebEditorScreenshotUtils::RestoreNodeSelection(const TSharedPtr<SGraphEditor>& InGraphEditor, const TSet<UObject*>& InSelectedNodes)
{
	for (UObject* NodeObject : InSelectedNodes)
	{
		if (UEdGraphNode* SelectedNode = Cast<UEdGraphNode>(NodeObject))
		{
			InGraphEditor->SetNodeSelection(SelectedNode, true);
		}
	}
}

bool FThirdwebEditorScreenshotUtils::HasSelectedNodes(const TSet<TSharedPtr<SGraphEditor>>& InGraphEditors)
{
	for (const TSharedPtr<SGraphEditor>& GraphEditor : InGraphEditors)
	{
		if (HasSelectedNodes(GraphEditor))
		{
			return true;
		}
	}
	return false;
}

bool FThirdwebEditorScreenshotUtils::HasSelectedNodes(const TSharedPtr<SGraphEditor>& InGraphEditor) { return InGraphEditor.IsValid() && !InGraphEditor->GetSelectedNodes().IsEmpty(); }

void FThirdwebEditorScreenshotUtils::ShowNotification(const TArray<FString>& InPaths)
{
	static FTextFormat NotificationFormat = LOCTEXT("NotificationFormat", "{0}|plural(one=Screenshot,other=Screenshots) taken: ");
	
	if (UThirdwebEditorSettings::NotificationsEnabled())
	{
		checkf(InPaths.Num() > 0, TEXT("InPaths must not be empty"));

		TArray<FString> FullPaths;
		Algo::Transform(InPaths, FullPaths, [](const FString& Path) { return FPaths::ConvertRelativePathToFull(Path); });
		
		FNotificationInfo NotificationInfo(FText::Format(NotificationFormat, {InPaths.Num()}));
		NotificationInfo.ExpireDuration = UThirdwebEditorSettings::GetScreenshotNotificationTimeoutSeconds();
		NotificationInfo.bUseSuccessFailIcons = true;
		NotificationInfo.HyperlinkText = FText::FromString(UKismetStringLibrary::JoinStringArray(FullPaths, TEXT("\n")));
		NotificationInfo.Hyperlink = FSimpleDelegate::CreateLambda([FullPaths] { FPlatformProcess::ExploreFolder(*FullPaths[0]); });

		FSlateNotificationManager::Get().AddNotification(NotificationInfo)->SetCompletionState(SNotificationItem::CS_Success);
	}
}

UTextureRenderTarget2D* FThirdwebEditorScreenshotUtils::DrawGraphEditor(const TSharedPtr<SGraphEditor>& InGraphEditor, const FVector2D& InWindowSize)
{
	if (UTextureRenderTarget2D* RenderTarget = FWidgetRenderer::CreateTargetFor(InWindowSize, Filter, bUseGamma); ensureMsgf(IsValid(RenderTarget), TEXT("RenderTarget is not valid")))
	{
		if (bUseGamma)
		{
			RenderTarget->bForceLinearGamma = true;
			RenderTarget->UpdateResourceImmediate(true);
		}

		FWidgetRenderer* WidgetRenderer = new FWidgetRenderer(bUseGamma, true);

		for (int32 Count = 0; Count < DrawTimes; Count++)
		{
			WidgetRenderer->DrawWidget(RenderTarget, InGraphEditor.ToSharedRef(), 1.f, InWindowSize, 0.f);
			FlushRenderingCommands();
		}

		BeginCleanup(WidgetRenderer);

		return RenderTarget;
	}

	return nullptr;
}

FString FThirdwebEditorScreenshotUtils::GetExtension(const EThirdwebEditorBlueprintImageFormat InFormat)
{
	switch (InFormat)
	{
	case EThirdwebEditorBlueprintImageFormat::PNG: return TEXT("png");
	case EThirdwebEditorBlueprintImageFormat::JPEG: return TEXT("jpg");
	case EThirdwebEditorBlueprintImageFormat::BMP: return TEXT("bmp");
	default:
		checkf(false, TEXT("Unknown image format"));
		return TEXT("png");
	}
}

FString FThirdwebEditorScreenshotUtils::GenerateName(const TSharedPtr<SGraphEditor>& InGraphEditor)
{
	if (ensure(InGraphEditor.IsValid()))
	{
		if (const UEdGraph* GraphObject = InGraphEditor->GetCurrentGraph(); IsValid(GraphObject))
		{
			if (const UObject* GraphOwner = GraphObject->GetOuter())
			{
				return FString::Printf(TEXT("%s_%s_"), *GraphOwner->GetName(), *GraphObject->GetName());
			}
		}
	}
	return TEXT("");
}

TSet<TSharedPtr<SGraphEditor>> FThirdwebEditorScreenshotUtils::FindGraphEditors()
{
	if (const TSharedPtr<SWindow> Window = FSlateApplication::Get().GetActiveTopLevelWindow(); Window.IsValid())
	{
		return FindGraphEditors(Window.ToSharedRef());
	}
	return TSet<TSharedPtr<SGraphEditor>>();
}

TSet<TSharedPtr<SGraphEditor>> FThirdwebEditorScreenshotUtils::FindGraphEditors(const TSharedRef<SWindow>& InWindow)
{
	const TSet<TSharedPtr<SWidget>> Widgets = FindChildren(InWindow, TEXT("SGraphEditorImpl"));
	TArray<TSharedPtr<SGraphEditor>> GraphEditors;

	Algo::Transform(Widgets, GraphEditors, [](const TSharedPtr<SWidget>& Widget) { return StaticCastSharedPtr<SGraphEditor>(Widget); });
	
	int32 _ = Algo::RemoveIf(GraphEditors, [](const TSharedPtr<SGraphEditor>& GraphEditor) { return !GraphEditor.IsValid(); });
	return TSet(GraphEditors);
}

TSet<TSharedPtr<SWidget>> FThirdwebEditorScreenshotUtils::FindChildren(const TSharedPtr<SWidget>& InWidget, const FName& InChildWidgetType)
{
	if (InWidget.IsValid())
	{
		if (FChildren* Children = InWidget->GetChildren(); Children && Children->Num() > 0)
		{
			TSet<TSharedPtr<SWidget>> Result;
			for (int32 i = 0; i < Children->Num(); i++)
			{
				TSharedRef<SWidget> Child = Children->GetChildAt(i);
				if (Child->GetType().IsEqual(InChildWidgetType))
				{
					Result.Add(Child);
				}
				if (TSet<TSharedPtr<SWidget>> Widgets = FindChildren(Child, InChildWidgetType); !Widgets.IsEmpty())
				{
					Result.Append(Widgets);
				}
			}
			return Result;
		}
	}

	return TSet<TSharedPtr<SWidget>>();
}

#undef LOCTEXT_NAMESPACE 