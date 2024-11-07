// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "ThirdwebEditorModule.h"

#include "ISettingsModule.h"
#include "ThirdwebEditorCommands.h"
#include "ThirdwebEditorCommon.h"
#include "ThirdwebEditorLog.h"
#include "ThirdwebEditorScreenshotUtils.h"
#include "ThirdwebEditorSettings.h"
#include "ThirdwebEditorStyle.h"
#include "ThirdwebRuntimeSettings.h"
#include "ToolMenus.h"
#include "Algo/ForEach.h"
#include "Misc/MessageDialog.h"

#define LOCTEXT_NAMESPACE "FThirdwebEditorModule"

void FThirdwebEditorModule::StartupModule()
{
	FThirdwebEditorStyle::Initialize();
	FThirdwebEditorStyle::ReloadTextures();

	FThirdwebEditorCommands::Register();

	CommandList = MakeShareable(new FUICommandList);

	CommandList->MapAction(
		FThirdwebEditorCommands::GetOpenSettingsCommand(),
		FExecuteAction::CreateStatic(&FThirdwebEditorModule::OpenSettingsButtonClicked),
		FCanExecuteAction()
	);
	CommandList->MapAction(
		FThirdwebEditorCommands::GetTakeScreenshotCommand(),
		FExecuteAction::CreateStatic(&FThirdwebEditorModule::TakeScreenshotButtonClicked),
		FCanExecuteAction()
	);

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FThirdwebEditorModule::RegisterMenus));
}

void FThirdwebEditorModule::ShutdownModule()
{
	UToolMenus::UnRegisterStartupCallback(this);
	UToolMenus::UnregisterOwner(this);

	FThirdwebEditorStyle::Shutdown();

	FThirdwebEditorCommands::Unregister();
}

void FThirdwebEditorModule::OpenSettingsButtonClicked()
{
	const UThirdwebRuntimeSettings* Settings = UThirdwebRuntimeSettings::Get();
	FModuleManager::LoadModuleChecked<ISettingsModule>("Settings").ShowViewer(Settings->GetContainerName(), Settings->GetCategoryName(), Settings->GetSectionName());
}

void FThirdwebEditorModule::TakeScreenshotButtonClicked()
{
	FThirdwebEditorScreenshotUtils::TakeScreenshot();
}

void FThirdwebEditorModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);
	TSharedPtr<FUICommandList> Cl = CommandList;
	{
		FToolMenuSection& Section = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar")->FindOrAddSection("ThirdwebSection");
		Section.AddEntry(FToolMenuEntry::InitToolBarButton(FThirdwebEditorCommands::GetOpenSettingsCommand())).SetCommandList(Cl);
	}
	{
		FToolMenuSection& Section = UToolMenus::Get()->ExtendMenu("AssetEditor.WidgetBlueprintEditor.ToolBar")->FindOrAddSection("ThirdwebSection");
		FThirdwebEditorCommands::ForEach([&Section, Cl](const TSharedPtr<FUICommandInfo>& Ci) { Section.AddEntry(FToolMenuEntry::InitToolBarButton(Ci)).SetCommandList(Cl); });
	}
	
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FThirdwebEditorModule, ThirdwebEditor)
