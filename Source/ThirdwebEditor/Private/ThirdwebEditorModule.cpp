// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "ThirdwebEditorModule.h"

#include "ISettingsModule.h"
#include "ThirdwebEditorCommands.h"
#include "ThirdwebEditorCommon.h"
#include "ThirdwebEditorLog.h"
#include "ThirdwebEditorScreenshotUtils.h"
#include "ThirdwebEditorSettings.h"
#include "ThirdwebEditorStyle.h"
#include "ThirdwebEditorUtils.h"
#include "ThirdwebRuntimeSettings.h"
#include "ToolMenus.h"
#include "Algo/ForEach.h"
#include "Checks/TWECheckEngine.h"
#include "Internationalization/Text.h"
#include "Misc/MessageDialog.h"

#define LOCTEXT_NAMESPACE "FThirdwebEditorModule"

void FThirdwebEditorModule::StartupModule()
{
	FThirdwebEditorStyle::Initialize();
	FThirdwebEditorStyle::ReloadTextures();
	FThirdwebEditorCommands::Register();

	CommandList = MakeShareable(new FUICommandList);
	
	// Configuration
	CommandList->MapAction(
		FThirdwebEditorCommands::Get().OpenRuntimeSettings,
		FExecuteAction::CreateStatic(&FThirdwebEditorModule::OpenRuntimeSettings),
		FCanExecuteAction()
	);
	CommandList->MapAction(
		FThirdwebEditorCommands::Get().OpenEditorSettings,
		FExecuteAction::CreateStatic(&FThirdwebEditorModule::OpenEditorSettings),
		FCanExecuteAction()
	);
	// Utilities
	CommandList->MapAction(
		FThirdwebEditorCommands::Get().TakeScreenshot,
		FExecuteAction::CreateStatic(&FThirdwebEditorModule::TakeScreenshot),
		FCanExecuteAction()
	);
	// Reference
	CommandList->MapAction(
		FThirdwebEditorCommands::Get().ViewDocumentation,
		FExecuteAction::CreateStatic(&FThirdwebEditorModule::ViewDocumentation),
		FCanExecuteAction()
	);
	// Support
	CommandList->MapAction(
		FThirdwebEditorCommands::Get().AccessOfficialSupport,
		FExecuteAction::CreateStatic(&FThirdwebEditorModule::AccessOfficialSupport),
		FCanExecuteAction()
	);
	CommandList->MapAction(
		FThirdwebEditorCommands::Get().AccessCommunitySupport,
		FExecuteAction::CreateStatic(&FThirdwebEditorModule::AccessCommunitySupport),
		FCanExecuteAction()
	);
	CommandList->MapAction(
		FThirdwebEditorCommands::Get().ReportABug,
		FExecuteAction::CreateStatic(&FThirdwebEditorModule::ReportABug),
		FCanExecuteAction()
	);
	CommandList->MapAction(
		FThirdwebEditorCommands::Get().IssueTracker,
		FExecuteAction::CreateStatic(&FThirdwebEditorModule::IssueTracker),
		FCanExecuteAction()
	);
	// Footer
	CommandList->MapAction(
		FThirdwebEditorCommands::Get().ViewWebsite,
		FExecuteAction::CreateStatic(&FThirdwebEditorModule::ViewWebsite),
		FCanExecuteAction()
	);

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FThirdwebEditorModule::RegisterMenus));

	CheckEngine = MakeShared<FTWECheckEngine>();

	// Register ticker.
	TickerHandle = FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateRaw(this, &FThirdwebEditorModule::Tick));
}

void FThirdwebEditorModule::ShutdownModule()
{
	FTSTicker::GetCoreTicker().RemoveTicker(TickerHandle);
	TickerHandle.Reset();
	UToolMenus::UnRegisterStartupCallback(this);
	UToolMenus::UnregisterOwner(this);
	FThirdwebEditorStyle::Shutdown();
	FThirdwebEditorCommands::Unregister();
}

bool FThirdwebEditorModule::Tick(float DeltaSeconds)
{
	CheckEngine->Tick(DeltaSeconds);
	return true;
}

void FThirdwebEditorModule::ProcessLogMessage(const FString& InLogLevel, const FString& Category, const FString& Message)
{
	CheckEngine->ProcessLogMessage(InLogLevel, Category, Message);
}

void FThirdwebEditorModule::ProcessCustomSignal(const FString& Context, const FString& SignalId)
{
	CheckEngine->ProcessCustomSignal(Context, SignalId);
}

void FThirdwebEditorModule::OpenRuntimeSettings()
{
	ThirdwebEditorUtils::OpenDeveloperSettings<UThirdwebRuntimeSettings>();
}

void FThirdwebEditorModule::OpenEditorSettings()
{
	ThirdwebEditorUtils::OpenDeveloperSettings<UThirdwebEditorSettings>();
}

void FThirdwebEditorModule::ReportABug()
{
	FPlatformProcess::LaunchURL(TEXT("https://github.com/thirdweb-dev/unreal-engine/issues/new/choose"), nullptr, nullptr);
}

void FThirdwebEditorModule::IssueTracker()
{
	FPlatformProcess::LaunchURL(TEXT("https://github.com/thirdweb-dev/unreal-engine/issues?q=sort%3Aupdated-desc+is%3Aissue+is%3Aopen"), nullptr, nullptr);
}

void FThirdwebEditorModule::TakeScreenshot()
{
	FThirdwebEditorScreenshotUtils::TakeScreenshot();
}

void FThirdwebEditorModule::RegisterMenus()
{
	TSharedPtr<FUICommandInfo> AccessOfficialSupport;
	TSharedPtr<FUICommandInfo> AccessCommunitySupport;
	TSharedPtr<FUICommandInfo> ReportABug;
	TSharedPtr<FUICommandInfo> IssueTracker;
	// Footer
	TSharedPtr<FUICommandInfo> ViewWebsite;
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);
	{
		UToolMenu* Menu = UToolMenus::Get()->RegisterMenu("LevelEditor.LevelEditorToolBar.LevelToolbarThirdweb");

		// Configuration
		{
			FToolMenuSection& Section = Menu->AddSection("ConfigurationSection", LOCTEXT("ConfigurationSection", "Configuration"));
			Section.AddMenuEntry(
				FThirdwebEditorCommands::Get().OpenRuntimeSettings,
				TAttribute<FText>(),
				TAttribute<FText>(),
				FSlateIcon(FAppStyle::GetAppStyleSetName(), "ProjectSettings.TabIcon")
			);
			Section.AddMenuEntry(
				FThirdwebEditorCommands::Get().OpenEditorSettings,
				TAttribute<FText>(),
				TAttribute<FText>(),
				FSlateIcon(FAppStyle::GetAppStyleSetName(), "EditorPreferences.TabIcon")
			);
		}
		// Utilities
		{
			FToolMenuSection& Section = Menu->AddSection("UtilitiesSection", LOCTEXT("UtilitiesSection", "Utilities"));

			Section.AddMenuEntry(
				FThirdwebEditorCommands::Get().TakeScreenshot,
				TAttribute<FText>(),
				TAttribute<FText>(),
				FSlateIcon(FThirdwebEditorStyle::GetStyleSetName(), "LevelEditor.Thirdweb.Screenshot")
			);
		}
		// Reference
		{
			FToolMenuSection& Section = Menu->AddSection("ReferenceSection", LOCTEXT("ReferenceSection", "Reference"));

			Section.AddMenuEntry(
				FThirdwebEditorCommands::Get().ViewDocumentation,
				TAttribute<FText>(),
				TAttribute<FText>(),
				FSlateIcon(FAppStyle::GetAppStyleSetName(), "Icons.Documentation")
			);
		}
		// Support
		{
			FToolMenuSection& Section = Menu->AddSection("SupportSection", LOCTEXT("SupportSection", "Support"));
			Section.AddMenuEntry(
				FThirdwebEditorCommands::Get().AccessOfficialSupport,
				TAttribute<FText>(),
				TAttribute<FText>(),
				FSlateIcon(FAppStyle::GetAppStyleSetName(), "MainFrame.VisitSupportWebSite")
			);
			Section.AddMenuEntry(
				FThirdwebEditorCommands::Get().AccessCommunitySupport,
				TAttribute<FText>(),
				TAttribute<FText>(),
				FSlateIcon(FAppStyle::GetAppStyleSetName(), "MainFrame.VisitCommunityHome")
			);
			Section.AddMenuEntry(
				FThirdwebEditorCommands::Get().ReportABug,
				TAttribute<FText>(),
				TAttribute<FText>(),
				FSlateIcon(FAppStyle::GetAppStyleSetName(), "Debug")
			);
			Section.AddMenuEntry(
				FThirdwebEditorCommands::Get().IssueTracker,
				TAttribute<FText>(),
				TAttribute<FText>(),
				FSlateIcon(FAppStyle::GetAppStyleSetName(), "MainFrame.OpenIssueTracker")
			);
			// Footer
			Section.AddSeparator("footer"),
				Section.AddMenuEntry(
					FThirdwebEditorCommands::Get().ViewWebsite,
					TAttribute<FText>(),
					TAttribute<FText>(),
					FSlateIcon(FAppStyle::GetAppStyleSetName(), "Icons.OpenInBrowser")
				);
		}
	}
	{
		UToolMenu* Toolbar = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
		{
			FToolMenuSection& SettingsSection = Toolbar->AddSection("ThirdwebSettings");
			{
				FUIAction Action;
				Action.IsActionVisibleDelegate = FIsActionButtonVisible::CreateLambda([]()
				{
					const UThirdwebEditorSettings* Settings = UThirdwebEditorSettings::Get();
					return !Settings || !Settings->HideDropdownInEditorToolbar();
				});
				FToolMenuEntry ComboButton = FToolMenuEntry::InitComboButton(
					"LevelToolbarThirdweb",
					Action,
					FOnGetContent::CreateStatic(
						&FThirdwebEditorModule::GenerateOnlineSettingsMenu,
						CommandList.ToSharedRef()),
					LOCTEXT("SettingsCombo", "Thirdweb"),
					LOCTEXT("SettingsCombo_ToolTip", "Manage the Thirdweb SDK"),
					FSlateIcon(FThirdwebEditorStyle::GetStyleSetName(), "LevelEditor.Thirdweb.Logo"),
					false,
					"LevelToolbarThirdweb");
				ComboButton.StyleNameOverride = "CalloutToolbar";
				SettingsSection.AddEntry(ComboButton);
			}
		}
	}
}

void FThirdwebEditorModule::ViewDocumentation()
{
	FPlatformProcess::LaunchURL(TEXT("https://portal.thirdweb.com/unreal-engine"), nullptr, nullptr);
}

void FThirdwebEditorModule::ViewWebsite()
{
	FPlatformProcess::LaunchURL(TEXT("https://thirdweb.com/"), nullptr, nullptr);
}

void FThirdwebEditorModule::AccessOfficialSupport()
{
	FPlatformProcess::LaunchURL(TEXT("https://thirdweb.com/support"), nullptr, nullptr);
}

void FThirdwebEditorModule::AccessCommunitySupport()
{
	FPlatformProcess::LaunchURL(TEXT("https://discord.gg/thirdweb"), nullptr, nullptr);
}

// ReSharper disable once CppPassValueParameterByConstReference
TSharedRef<class SWidget> FThirdwebEditorModule::GenerateOnlineSettingsMenu(TSharedRef<class FUICommandList> InCommandList)
{
	FToolMenuContext MenuContext(InCommandList);
	return UToolMenus::Get()->GenerateWidget("LevelEditor.LevelEditorToolBar.LevelToolbarThirdweb", MenuContext);
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FThirdwebEditorModule, ThirdwebEditor)
