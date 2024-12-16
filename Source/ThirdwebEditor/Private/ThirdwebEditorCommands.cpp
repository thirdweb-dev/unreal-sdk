// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "ThirdwebEditorCommands.h"

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
	// Configuration
	UI_COMMAND(OpenRuntimeSettings, "Project Settings...", "Edit core SDK runtime settings.", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(OpenEditorSettings, "Editor Preferences...", "Edit optional developer settings.", EUserInterfaceActionType::Button, FInputChord());
	// Utilities
	UI_COMMAND(TakeScreenshot, "Take Screenshot", "Take a screenshot of the currently selected blueprint nodes.", EUserInterfaceActionType::Button, FInputChord());
	// Reference
	UI_COMMAND(ViewDocumentation, "View Documentation", "View the plugin documentation.", EUserInterfaceActionType::Button, FInputChord());
	// Support
	UI_COMMAND(AccessOfficialSupport, "Official Support", "Get support from thirdweb.", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(AccessCommunitySupport, "Community Support", "Get support from the community.", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(ReportABug, "Report a Bug", "Found a bug? Let us know about it!", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(IssueTracker, "Issue Tracker", "Check the current status of public bugs and other issues.", EUserInterfaceActionType::Button, FInputChord());
	// Footer
	UI_COMMAND(ViewWebsite, "Visit thirdweb.com", "Open the website for Thirdweb.", EUserInterfaceActionType::Button, FInputChord());

}

#undef LOCTEXT_NAMESPACE
