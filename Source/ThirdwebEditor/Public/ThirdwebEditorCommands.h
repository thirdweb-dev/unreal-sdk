// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "ThirdwebEditorStyle.h"
#include "Framework/Commands/Commands.h"

class FThirdwebEditorCommands : public TCommands<FThirdwebEditorCommands>
{
public:
	FThirdwebEditorCommands();

	virtual void RegisterCommands() override;

	// Configuration
	TSharedPtr<FUICommandInfo> OpenRuntimeSettings;
	TSharedPtr<FUICommandInfo> OpenEditorSettings;
	// Utilities
	TSharedPtr<FUICommandInfo> TakeScreenshot;
	// Reference
	TSharedPtr<FUICommandInfo> ViewDocumentation;
	// Support
	TSharedPtr<FUICommandInfo> AccessOfficialSupport;
	TSharedPtr<FUICommandInfo> AccessCommunitySupport;
	TSharedPtr<FUICommandInfo> ReportABug;
	TSharedPtr<FUICommandInfo> IssueTracker;
	// Footer
	TSharedPtr<FUICommandInfo> ViewWebsite;
};
