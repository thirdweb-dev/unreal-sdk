// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "ThirdwebEditorStyle.h"

#include "Framework/Commands/Commands.h"

class FThirdwebEditorCommands : public TCommands<FThirdwebEditorCommands>
{
public:
	FThirdwebEditorCommands()
		: TCommands(TEXT("ThirdwebEditor"), NSLOCTEXT("Contexts", "ThirdwebEditor", "ThirdwebEditor Plugin"), NAME_None, FThirdwebEditorStyle::GetStyleSetName())
	{
	}
	
	virtual void RegisterCommands() override;

public:
	TSharedPtr<FUICommandInfo> PluginAction;
};
