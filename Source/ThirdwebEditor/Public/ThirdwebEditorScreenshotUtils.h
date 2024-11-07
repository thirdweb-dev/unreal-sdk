// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

enum class EThirdwebEditorBlueprintImageFormat : uint8;
struct FThirdwebEditorScreenshotData;
struct FColor;
class UTextureRenderTarget2D;
class SGraphEditor;

class FThirdwebEditorScreenshotUtils
{
public:
	static void TakeScreenshot();

	static FString SaveScreenshot(const FThirdwebEditorScreenshotData& InData);
	static FThirdwebEditorScreenshotData CaptureGraphEditor(const TSharedPtr<SGraphEditor>& InGraphEditor);
	static void RestoreNodeSelection(const TSharedPtr<SGraphEditor>& InGraphEditor, const TSet<UObject*>& InSelectedNodes);
	static bool HasSelectedNodes(const TSet<TSharedPtr<SGraphEditor>>& InGraphEditors);
	static bool HasSelectedNodes(const TSharedPtr<SGraphEditor>& InGraphEditor);
	static void ShowNotification(const TArray<FString>& InPaths);
	static UTextureRenderTarget2D* DrawGraphEditor(const TSharedPtr<SGraphEditor>& InGraphEditor, const FVector2D& InWindowSize);
	static FString GetExtension(const EThirdwebEditorBlueprintImageFormat InFormat);
	static FString GenerateName(const TSharedPtr<SGraphEditor>& InGraphEditor);

	static TSet<TSharedPtr<SGraphEditor>> FindGraphEditors();
	static TSet<TSharedPtr<SGraphEditor>> FindGraphEditors(const TSharedRef<SWindow>& InWindow);
	static TSet<TSharedPtr<SWidget>> FindChildren(const TSharedPtr<SWidget>& InWidget, const FName& InChildWidgetType);
	
protected:
	static constexpr bool bUseGamma = true;
	static constexpr int DrawTimes = 2;
	static constexpr TextureFilter Filter = TF_Default;
};


