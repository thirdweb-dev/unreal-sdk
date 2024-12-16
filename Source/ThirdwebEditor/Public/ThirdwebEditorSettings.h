// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "Engine/DeveloperSettings.h"
#include "ThirdwebEditorSettings.generated.h"

enum class EThirdwebEditorBlueprintImageFormat : uint8;

/**
 * This class acts as a configuration holder to manage various settings required for the editor environment in the Thirdweb system.
 */
UCLASS(Config=EditorPerProjectUserSettings, meta=(DisplayName="Thirdweb Editor"))
class THIRDWEBEDITOR_API UThirdwebEditorSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UThirdwebEditorSettings();

#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual bool CanEditChange(const FProperty* InProperty) const override;
#endif

protected:
	UPROPERTY(Config, EditAnywhere, meta=(InlineEditConditionToggle), Category=Screenshots)
	bool bCustomScreenshotBaseName;

	UPROPERTY(Config, EditAnywhere, DisplayName="Base Name", meta=(EditCondition="bCustomScreenshotBaseName"), Category=Screenshots)
	FString CustomScreenshotBaseName;

	UPROPERTY(Config, EditAnywhere, DisplayName="Format", Category=Screenshots)
	EThirdwebEditorBlueprintImageFormat ScreenshotFormat;

	UPROPERTY(Config, EditAnywhere, DisplayName="Quality", meta=(EditCondition="ScreenshotFormat==EBSTImageFormat::JPEG", ClampMin="10", ClampMax="100", UIMin="10", UIMax="100"), Category=Screenshots)
	int32 ScreenshotQuality;

	UPROPERTY(Config, EditAnywhere, DisplayName="Save Directory", meta=(RelativePath), Category=Screenshots)
	FDirectoryPath ScreenshotSaveDirectory;

	UPROPERTY(Config, EditAnywhere, DisplayName="Padding", Category=Screenshots)
	int32 ScreenshotPadding;

	UPROPERTY(Config, EditAnywhere, DisplayName="Size Constraints", Category=Screenshots)
	FIntPoint ScreenshotSizeConstraints;

	UPROPERTY(Config, EditAnywhere, DisplayName="Zoom", meta=(ClampMin="0.1", ClampMax="2", UIMin="0.1", UIMax="2"), Category=Screenshots)
	float ScreenshotZoom;

	UPROPERTY(Config, EditAnywhere, DisplayName="Shortcut", meta=(ConfigRestartRequired=true), Category=Screenshots)
	FInputChord ScreenshotShortcut;

	UPROPERTY(Config, EditAnywhere, DisplayName="Disable Notifications", Category="Screenshots|Notifications")
	bool bScreenshotDisableNotifications;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, DisplayName="Timeout", meta=(EditCondition="!bScreenshotDisableNotifications", EditConditionHides), Category="Screenshots|Notifications")
	int32 ScreenshotNotificationTimeoutSeconds;

	UPROPERTY(Config, EditAnywhere, DisplayName="Hide Dropdown In Editor Toolbar", Category=Tools)
	bool bHideDropdownInEditorToolbar;

public:
	static bool NotificationsEnabled() { return !Get()->bScreenshotDisableNotifications; }
	static FInputChord GetScreenshotShortcut() { return Get()->ScreenshotShortcut; }
	static int32 GetScreenshotNotificationTimeoutSeconds() { return Get()->ScreenshotNotificationTimeoutSeconds; }
	static int32 GetScreenshotPadding() { return Get()->ScreenshotPadding; }
	static float GetScreenshotZoom() { return Get()->ScreenshotZoom; }
	static FIntPoint GetScreenshotSizeConstraints() { return Get()->ScreenshotSizeConstraints; }
	static bool HasCustomScreenshotBaseName() { return Get()->bCustomScreenshotBaseName; }
	static FString GetCustomScreenshotBaseName() { return Get()->CustomScreenshotBaseName; }
	static FDirectoryPath GetScreenshotSaveDirectory() { return Get()->ScreenshotSaveDirectory; }
	static EThirdwebEditorBlueprintImageFormat GetScreenshotFormat() { return Get()->ScreenshotFormat; }
	static float GetScreenshotQuality();
	static bool HideDropdownInEditorToolbar() { return Get()->bHideDropdownInEditorToolbar; }
	
	/** Convenience Getter */
	static UThirdwebEditorSettings* Get() { return GetMutableDefault<UThirdwebEditorSettings>(); }
};
