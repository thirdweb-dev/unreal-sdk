// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "Checks/TWECheckEngine.h"

#include "ILevelEditor.h"
#include "LevelEditor.h"
#include "SLevelViewport.h"
#include "Checks/TWEEssentialSettingsCheck.h"
#include "Modules/ModuleManager.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/SWidget.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"

FTWECheckEngine::FTWECheckEngine()
{
	InjectedNotificationBar = SNew(SVerticalBox);
	RegisteredChecks.Add(MakeShared<FTWEEssentialSettingsCheck>());
}

FTWECheckEngine::~FTWECheckEngine()
{
	if (CurrentlyInjectedIntoWidget.IsValid())
	{
		CurrentlyInjectedIntoWidget.Pin()->RemoveSlot(InjectedNotificationBar.ToSharedRef());
		InjectedNotificationBar.Reset();
		CurrentlyInjectedIntoWidget.Reset();
	}
}

FSlateColor FTWECheckEngine::GetNotificationBackgroundColor() const
{
	return FSlateColor(FLinearColor(0.89627f, 0.799103f, 0.219526f, 1.0f));
}

FSlateColor FTWECheckEngine::GetNotificationButtonOutlineColor() const
{
	return FSlateColor(FLinearColor(202.f / 255.f, 187.f / 255.f, 80.f / 255.f, 1.0f));
}

FSlateColor FTWECheckEngine::GetNotificationButtonBackgroundColor() const
{
	return FSlateColor(FLinearColor(255.f / 255.f, 244.f / 255.f, 143.f / 255.f, 1.0f));
}

FSlateColor FTWECheckEngine::GetNotificationButtonBackgroundHoveredColor() const
{
	return FSlateColor(FLinearColor(1.0f, 0.921569f, 0.392157f, 1.0f));
}

FSlateColor FTWECheckEngine::GetNotificationButtonBackgroundPressedColor() const
{
	return FSlateColor(FLinearColor(0.947307f, 0.854993f, 0.246201f, 1.0f));
}

FSlateColor FTWECheckEngine::GetNotificationFontColor() const
{
	return FSlateColor(FLinearColor(0.0f, 0.0f, 0.0f, 1.0f));
}

const FButtonStyle* FTWECheckEngine::GetNotificationButtonStyle() const
{
	auto Self = const_cast<FTWECheckEngine*>(this);
	Self->NotificationButtonStyle = FCoreStyle::Get().GetWidgetStyle<FButtonStyle>("Button");
	Self->NotificationButtonStyle.Normal.OutlineSettings.Color = GetNotificationButtonOutlineColor();
	Self->NotificationButtonStyle.Hovered.OutlineSettings.Color = GetNotificationButtonOutlineColor();
	Self->NotificationButtonStyle.Pressed.OutlineSettings.Color = GetNotificationButtonOutlineColor();
	Self->NotificationButtonStyle.Normal.TintColor = GetNotificationButtonBackgroundColor();
	Self->NotificationButtonStyle.Hovered.TintColor = GetNotificationButtonBackgroundHoveredColor();
	Self->NotificationButtonStyle.Pressed.TintColor = GetNotificationButtonBackgroundPressedColor();
	return &(Self->NotificationButtonStyle);
}

const FSlateBrush* FTWECheckEngine::GetNotificationBackgroundBrush() const
{
	auto Self = const_cast<FTWECheckEngine*>(this);
	Self->NotificationBackgroundBrush = FSlateBrush();
	Self->NotificationBackgroundBrush.ImageSize = FVector2D(32, 32);
	Self->NotificationBackgroundBrush.Margin = FMargin();
	Self->NotificationBackgroundBrush.TintColor = FLinearColor::White;
	Self->NotificationBackgroundBrush.DrawAs = ESlateBrushDrawType::Image;
	Self->NotificationBackgroundBrush.Tiling = ESlateBrushTileType::NoTile;
	Self->NotificationBackgroundBrush.Mirroring = ESlateBrushMirrorType::NoMirror;
	Self->NotificationBackgroundBrush.ImageType = ESlateBrushImageType::NoImage;
	return &(Self->NotificationBackgroundBrush);
}

TSharedRef<SBorder> FTWECheckEngine::CreateNewNotification(const FTWECheckEntry& Entry)
{
	auto HorizontalBox = SNew(SHorizontalBox);
	HorizontalBox->AddSlot().FillWidth(1).VAlign(
		VAlign_Center)[SNew(STextBlock)
	.Text(FText::FromString(Entry.GetCheckMessage()))
	.AutoWrapText(true)
	.ColorAndOpacity(this, &FTWECheckEngine::GetNotificationFontColor)];
	for (const auto& Action : Entry.GetCheckActions())
	{
		HorizontalBox->AddSlot()
		             .AutoWidth()
		             .Padding(FMargin(10, 0, 0, 0))
		             .VAlign(EVerticalAlignment::VAlign_Center)
		[SNew(SButton)
		.ButtonStyle(FTWECheckEngine::GetNotificationButtonStyle())
		.OnClicked(FOnClicked::CreateSP(
			this,
			&FTWECheckEngine::OnActionClicked,
			Entry.GetCheckId(),
			Action
			.GetActionId()))
		[
			SNew(STextBlock)
			.Text(FText::FromString(Action.GetActionDisplayName()))
			.ColorAndOpacity(this, &FTWECheckEngine::GetNotificationFontColor)
		]];
	}
	HorizontalBox->AddSlot()
	             .AutoWidth()
	             .Padding(FMargin(10, 0, 0, 0))
	             .VAlign(EVerticalAlignment::VAlign_Center)
	[SNew(SButton)
	.ButtonStyle(FTWECheckEngine::GetNotificationButtonStyle())
	.OnClicked(FOnClicked::CreateSP(this, &FTWECheckEngine::OnDismissClicked, Entry.GetCheckId()))
	[
		SNew(STextBlock)
		.Text(FText::FromString("Dismiss"))
		.ColorAndOpacity(this, &FTWECheckEngine::GetNotificationFontColor)
	]];

	return SNew(SBorder)
		.BorderBackgroundColor(this, &FTWECheckEngine::GetNotificationBackgroundColor)
		.BorderImage(this, &FTWECheckEngine::GetNotificationBackgroundBrush)
		[
			HorizontalBox
		]
		.Padding(FMargin(10, 5));
}

FReply FTWECheckEngine::OnActionClicked(
	// NOLINTNEXTLINE(performance-unnecessary-value-param)
	FString CheckId,
	// NOLINTNEXTLINE(performance-unnecessary-value-param)
	FString ActionId)
{
	if (!CurrentNotifications.Contains(CheckId))
	{
		return FReply::Handled();
	}

	CurrentNotifications[CheckId].Value->HandleAction(CheckId, ActionId);

	InjectedNotificationBar->RemoveSlot(CurrentNotifications[CheckId].Key.ToSharedRef());
	CurrentNotifications.Remove(CheckId);

	return FReply::Handled();
}

FReply FTWECheckEngine::OnDismissClicked(FString CheckId)
{
	if (!CurrentNotifications.Contains(CheckId))
	{
		return FReply::Handled();
	}

	InjectedNotificationBar->RemoveSlot(CurrentNotifications[CheckId].Key.ToSharedRef());
	CurrentNotifications.Remove(CheckId);

	DismissedCheckIds.Add(CheckId);

	return FReply::Handled();
}

void FTWECheckEngine::Tick(float DeltaSeconds)
{
	TSet<FString> DismissedCheckIdsToForget = DismissedCheckIds;
	for (const auto& Check : RegisteredChecks)
	{
		if (Check->ShouldTick())
		{
			auto Results = Check->Tick(DeltaSeconds);
			if (Results.Num() > 0)
			{
				for (const auto& Entry : Results)
				{
					if (!CurrentNotifications.Contains(Entry.GetCheckId()))
					{
						if (DismissedCheckIds.Contains(Entry.GetCheckId()))
						{
							DismissedCheckIdsToForget.Remove(Entry.GetCheckId());
						}
						else
						{
							TSharedRef<SBorder> Notification = CreateNewNotification(Entry);
							CurrentNotifications.Add(
								Entry.GetCheckId(),
								TTuple<TSharedPtr<SBorder>, TSharedPtr<ITWECheck>>(Notification, Check));
							InjectedNotificationBar->AddSlot()
							    .Padding(FMargin(0, 0, 0, 1))
							    .AutoHeight()
							    .AttachWidget(Notification);
						}
					}
				}
			}
		}
	}
	for (const auto& CheckId : DismissedCheckIdsToForget)
	{
		DismissedCheckIds.Remove(CheckId);
	}

	if (FLevelEditorModule* LevelEditorModule = FModuleManager::GetModulePtr<FLevelEditorModule>(TEXT("LevelEditor")))
	{
		if (TWeakPtr<ILevelEditor> LevelEditor = LevelEditorModule->GetLevelEditorInstance(); LevelEditor.IsValid())
		{
			if (TSharedPtr<ILevelEditor> PinnedLevelEditor = LevelEditor.Pin(); PinnedLevelEditor.IsValid())
			{
				TSharedPtr<SLevelViewport> CurrentActiveLevelViewport = PinnedLevelEditor->GetActiveViewportInterface();
				if (!CurrentActiveLevelViewport.IsValid())
				{
					if (PinnedLevelEditor->GetViewports().Num() > 0)
					{
						CurrentActiveLevelViewport = PinnedLevelEditor->GetViewports()[0];
					}
				}

				bool bRebuildInjectedLocation = false;
				if ((CurrentActiveLevelViewport.IsValid() && !ActiveLevelViewport.IsValid()) ||
					(CurrentActiveLevelViewport.Get() != ActiveLevelViewport.Pin().Get()))
				{
					bRebuildInjectedLocation = true;
					ActiveLevelViewport = CurrentActiveLevelViewport;
				}

				if (bRebuildInjectedLocation)
				{
					TSharedPtr<SWidget> CurrentWidget = StaticCastSharedPtr<SWidget>(CurrentActiveLevelViewport);
					while (CurrentWidget.IsValid() && !CurrentWidget->GetType().IsEqual(FName(TEXT("SVerticalBox"))) &&
						CurrentWidget->IsParentValid())
					{
						CurrentWidget = CurrentWidget->GetParentWidget();
					}

					if (CurrentWidget.IsValid() && CurrentWidget->GetType().IsEqual(FName(TEXT("SVerticalBox"))))
					{
						if (TSharedPtr<SVerticalBox> VerticalBox = StaticCastSharedPtr<SVerticalBox>(CurrentWidget); VerticalBox.IsValid())
						{
							if (VerticalBox->GetChildren()->Num() == 2)
							{
								if (CurrentlyInjectedIntoWidget.IsValid())
								{
									CurrentlyInjectedIntoWidget.Pin()->RemoveSlot(
										InjectedNotificationBar.ToSharedRef());
									CurrentlyInjectedIntoWidget.Reset();
								}

								VerticalBox->InsertSlot(1)
								           .AutoHeight()
								           .Padding(FMargin(0, 0, 0, 1))
								           .AttachWidget(InjectedNotificationBar.ToSharedRef());
								CurrentlyInjectedIntoWidget = VerticalBox;
							}
						}
					}
				}
			}
		}
	}
}

void FTWECheckEngine::ProcessLogMessage(const FString& InLogLevel, const FString& Category, const FString& Message)
{
	for (const auto& Check : RegisteredChecks)
	{
		auto Results = Check->ProcessLogMessage(InLogLevel, Category, Message);
		if (Results.Num() > 0)
		{
			for (const auto& Entry : Results)
			{
				if (!CurrentNotifications.Contains(Entry.GetCheckId()))
				{
					// We don't persist dismissals for log-based checks, since they aren't persistently triggered every
					// tick.
					DismissedCheckIds.Remove(Entry.GetCheckId());

					TSharedRef<SBorder> Notification = CreateNewNotification(Entry);
					CurrentNotifications.Add(
						Entry.GetCheckId(),
						TTuple<TSharedPtr<SBorder>, TSharedPtr<ITWECheck>>(Notification, Check));
					InjectedNotificationBar->AddSlot()
					    .Padding(FMargin(0, 0, 0, 1))
					    .AutoHeight()
					    .AttachWidget(Notification);
				}
			}
		}
	}
}

void FTWECheckEngine::ProcessCustomSignal(const FString& Context, const FString& SignalId)
{
	for (const auto& Check : RegisteredChecks)
	{
		auto Results = Check->ProcessCustomSignal(Context, SignalId);
		if (Results.Num() > 0)
		{
			for (const auto& Entry : Results)
			{
				if (!CurrentNotifications.Contains(Entry.GetCheckId()))
				{
					// We don't persist dismissals for log-based checks, since they aren't persistently triggered every tick.
					DismissedCheckIds.Remove(Entry.GetCheckId());

					TSharedRef<SBorder> Notification = CreateNewNotification(Entry);
					CurrentNotifications.Add(
						Entry.GetCheckId(),
						TTuple<TSharedPtr<SBorder>, TSharedPtr<ITWECheck>>(Notification, Check));
					InjectedNotificationBar->AddSlot()
					    .Padding(FMargin(0, 0, 0, 1))
					    .AutoHeight()
					    .AttachWidget(Notification);
				}
			}
		}
	}
}
