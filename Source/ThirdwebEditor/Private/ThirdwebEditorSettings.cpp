// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "ThirdwebEditorSettings.h"

#include "ThirdwebEditorCommon.h"

UThirdwebEditorSettings::UThirdwebEditorSettings()
{
	bCustomScreenshotBaseName = false;
	CustomScreenshotBaseName = TEXT("BlueprintGraphScreenshot");
	ScreenshotFormat = EThirdwebEditorBlueprintImageFormat::PNG;
	ScreenshotQuality = 100;
	ScreenshotSaveDirectory = FDirectoryPath(FPaths::ScreenShotDir());
	ScreenshotPadding = 128.0f;
	ScreenshotSizeConstraints = FIntPoint(128, 15360);
	ScreenshotZoom = 1.0f;
	ScreenshotShortcut = FInputChord(EModifierKey::Control, EKeys::F7);
	bScreenshotDisableNotifications = false;
	ScreenshotNotificationTimeoutSeconds = 5;
}

#if WITH_EDITOR
void UThirdwebEditorSettings::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
}

bool UThirdwebEditorSettings::CanEditChange(const FProperty* InProperty) const
{
	return Super::CanEditChange(InProperty);
}

float UThirdwebEditorSettings::GetScreenshotQuality() { return GetScreenshotFormat() == EThirdwebEditorBlueprintImageFormat::JPEG ? Get()->ScreenshotQuality : 0.0f; }

#endif
