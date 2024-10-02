// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"


class FThirdwebEditorModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
private:
	void RegisterMenus();

public:
	void EditorToolbarButtonClicked();
	
private:
	TSharedPtr<FUICommandList> PluginCommands;
};
