// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "ITWECheck.h"
#include "Styling/SlateBrush.h"
#include "Styling/SlateColor.h"
#include "Styling/SlateTypes.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Layout/SBorder.h"

class FTWECheckEngine : public TSharedFromThis<FTWECheckEngine>
{
	// These widgets are managed by the level editor, so must be TWeakPtr to allow level editor UI to be freed correctly.
	TWeakPtr<class SLevelViewport> ActiveLevelViewport;
	TWeakPtr<SVerticalBox> CurrentlyInjectedIntoWidget;

	TSharedPtr<SVerticalBox> InjectedNotificationBar;

	FButtonStyle NotificationButtonStyle;
	FSlateBrush NotificationBackgroundBrush;

	FSlateColor GetNotificationBackgroundColor() const;
	FSlateColor GetNotificationButtonOutlineColor() const;
	FSlateColor GetNotificationButtonBackgroundColor() const;
	FSlateColor GetNotificationButtonBackgroundHoveredColor() const;
	FSlateColor GetNotificationButtonBackgroundPressedColor() const;
	FSlateColor GetNotificationFontColor() const;
	const FButtonStyle* GetNotificationButtonStyle() const;
	const FSlateBrush* GetNotificationBackgroundBrush() const;

	TSharedRef<SBorder> CreateNewNotification(const FTWECheckEntry& Entry);
	FReply OnActionClicked(FString CheckId, FString ActionId);
	FReply OnDismissClicked(FString CheckId);

	TMap<FString, TTuple<TSharedPtr<SBorder>, TSharedPtr<ITWECheck>>> CurrentNotifications;

	TArray<TSharedRef<ITWECheck>> RegisteredChecks;
	TSet<FString> DismissedCheckIds;

public:
	FTWECheckEngine();
	UE_NONCOPYABLE(FTWECheckEngine);
	virtual ~FTWECheckEngine();

	void Tick(float DeltaSeconds);
	void ProcessLogMessage(const FString& InLogLevel, const FString& Category, const FString& Message);
	void ProcessCustomSignal(const FString& Context, const FString& SignalId);
};
