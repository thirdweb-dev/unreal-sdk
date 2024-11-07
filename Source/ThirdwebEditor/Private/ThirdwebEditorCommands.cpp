// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "ThirdwebEditorCommands.h"

#include "ThirdwebEditorSettings.h"

#define LOCTEXT_NAMESPACE "FThirdwebEditorModule"

FThirdwebEditorCommands::FThirdwebEditorCommands(): TCommands(
	TEXT("ThirdwebEditor"),
	NSLOCTEXT("Contexts", "ThirdwebEditor", "ThirdwebEditor Plugin"),
	NAME_None,
	FThirdwebEditorStyle::GetStyleSetName()
)
{
}

void FThirdwebEditorCommands::RegisterCommands()
{
	UI_COMMAND(OpenSettings, "Thirdweb Settings", "Open Thirdweb settings", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(TakeScreenshot, "Take Screenshot", "Take a screenshot of the currently selected blueprint nodes", EUserInterfaceActionType::Button, UThirdwebEditorSettings::GetScreenshotShortcut());
}

#undef LOCTEXT_NAMESPACE
