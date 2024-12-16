// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "Containers/Ticker.h"
#include "Modules/ModuleManager.h"

class FThirdwebEditorModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	bool Tick(float DeltaSeconds);

	void ProcessLogMessage(const FString& InLogLevel, const FString& Category, const FString& Message);
	void ProcessCustomSignal(const FString& Context, const FString& SignalId);

private:
	void RegisterMenus();

	// Configuration
	static void OpenRuntimeSettings();
	static void OpenEditorSettings();
	// Utilities
	static void TakeScreenshot();
	// Reference
	static void ViewDocumentation();
	// Support
	static void AccessOfficialSupport();
	static void AccessCommunitySupport();
	static void ReportABug();
	static void IssueTracker();
	// Footer
	static void ViewWebsite();
	
	static TSharedRef<class SWidget> GenerateOnlineSettingsMenu(TSharedRef<class FUICommandList> InCommandList);
	
	TSharedPtr<class FUICommandList> CommandList;
	TSharedPtr<class FTWECheckEngine> CheckEngine;
	FTSTicker::FDelegateHandle TickerHandle;
};
