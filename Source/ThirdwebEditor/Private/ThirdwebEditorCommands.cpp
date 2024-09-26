// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "ThirdwebEditorCommands.h"

#define LOCTEXT_NAMESPACE "FThirdwebEditorModule"

void FThirdwebEditorCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "Thirdweb", "Thirdweb Settings", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
