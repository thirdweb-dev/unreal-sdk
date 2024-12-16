// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "Checks/TWEEssentialSettingsCheck.h"

#include "ISettingsModule.h"
#include "ThirdwebEditorUtils.h"
#include "ThirdwebRuntimeSettings.h"
#include "UnrealEdMisc.h"
#include "Framework/Docking/TabManager.h"
#include "Widgets/Docking/SDockTab.h"

TArray<FTWECheckEntry> FTWEEssentialSettingsCheck::Tick(float DeltaSeconds) const
{
	// ReSharper disable once CppTooWideScopeInitStatement
	TSharedPtr<SDockTab> ExistingProjectSettingsTab = FGlobalTabmanager::Get()->FindExistingLiveTab(FName("ProjectSettings"));
	if (!ExistingProjectSettingsTab.IsValid() || (!ExistingProjectSettingsTab->IsActive() && !ExistingProjectSettingsTab->IsForeground()))
	{
		if (UThirdwebRuntimeSettings::GetClientId().IsEmpty() || UThirdwebRuntimeSettings::GetBundleId().IsEmpty())
		{
			return TArray<FTWECheckEntry>{
				FTWECheckEntry(
					"FTWEEssentialSettingsCheck::Misconfigured",
					"You are missing essential settings that are required for ThirdwebSDK to work. You must set values for at least \"Client ID\" and \"Bundle ID\".",
					TArray<FTWECheckAction>{
						FTWECheckAction("FTWEEssentialSettingsCheck::OpenProjectSettings", "Open settings"),
						FTWECheckAction("FTWEEssentialSettingsCheck::OpenDocumentation", "Read documentation")
					})
			};
		}
	}

	return EmptyEntries;
}

void FTWEEssentialSettingsCheck::HandleAction(const FString& CheckId, const FString& ActionId) const
{
	if (CheckId == "FTWEEssentialSettingsCheck::Misconfigured" ||
		CheckId == "FTWEEssentialSettingsCheck::EditorRestartRequired")
	{
		if (ActionId == "FTWEEssentialSettingsCheck::OpenProjectSettings")
		{
			ThirdwebEditorUtils::OpenDeveloperSettings<UThirdwebRuntimeSettings>();
		}
		else if (ActionId == "FTWEEssentialSettingsCheck::RestartEditor")
		{
			FUnrealEdMisc::Get().RestartEditor(false);
		}
		else if (ActionId == "FTWEEssentialSettingsCheck::OpenDocumentation")
		{
			FPlatformProcess::LaunchURL(TEXT("https://portal.thirdweb.com/unreal-engine/getting-started"), nullptr, nullptr);
		}
	}
}
