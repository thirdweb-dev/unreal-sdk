// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "ThirdwebEditorModule.h"

#include "ISettingsModule.h"
#include "ThirdwebEditorCommands.h"
#include "ThirdwebEditorStyle.h"
#include "ThirdwebRuntimeSettings.h"
#include "ToolMenus.h"

#include "Misc/MessageDialog.h"

#define LOCTEXT_NAMESPACE "FThirdwebEditorModule"

void FThirdwebEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	FThirdwebEditorStyle::Initialize();
	FThirdwebEditorStyle::ReloadTextures();

	FThirdwebEditorCommands::Register();

	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FThirdwebEditorCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FThirdwebEditorModule::EditorToolbarButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FThirdwebEditorModule::RegisterMenus));
}

void FThirdwebEditorModule::ShutdownModule()
{
	UToolMenus::UnRegisterStartupCallback(this);
	UToolMenus::UnregisterOwner(this);
	
	FThirdwebEditorStyle::Shutdown();
	
	FThirdwebEditorCommands::Unregister();
}

// ReSharper disable once CppMemberFunctionMayBeStatic
void FThirdwebEditorModule::EditorToolbarButtonClicked()
{
	const auto Settings = GetDefault<UThirdwebRuntimeSettings>();
	FModuleManager::LoadModuleChecked<ISettingsModule>("Settings").ShowViewer(Settings->GetContainerName(), Settings->GetCategoryName(), Settings->GetSectionName());
}

void FThirdwebEditorModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FThirdwebEditorCommands::Get().PluginAction, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("PluginTools");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FThirdwebEditorCommands::Get().PluginAction));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FThirdwebEditorModule, ThirdwebEditor)
