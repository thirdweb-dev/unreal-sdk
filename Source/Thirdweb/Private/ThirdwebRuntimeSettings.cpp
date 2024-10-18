// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "ThirdwebRuntimeSettings.h"

#include "ThirdwebCommon.h"
#include "ThirdwebInternal.h"
#include "ThirdwebLog.h"

#include "HAL/FileManager.h"

#include "Misc/Paths.h"

UThirdwebRuntimeSettings::UThirdwebRuntimeSettings()
{
	bSendAnalytics = true;
	bOverrideOAuthBrowserProviderBackends = false;
	OAuthBrowserProviderBackendOverrides[static_cast<int>(EThirdwebOAuthProvider::Google)] = EThirdwebOAuthBrowserBackend::External;
	OAuthBrowserProviderBackendOverrides[static_cast<int>(EThirdwebOAuthProvider::Telegram)] = EThirdwebOAuthBrowserBackend::External;
}

void UThirdwebRuntimeSettings::GenerateEncryptionKey()
{
#if WITH_EDITOR
	EncryptionKey = FThirdwebAnalytics::GenerateUUID();
	if (MarkPackageDirty())
	{
		PostEditChange();
	}
#endif
}

TArray<FString> UThirdwebRuntimeSettings::GetThirdwebGlobalEngineSigners()
{
	if (const UThirdwebRuntimeSettings* Settings = Get())
	{
		return Settings->EngineSigners;
	}
	return {};
}

FString UThirdwebRuntimeSettings::GetThirdwebGlobalEngineSigner(bool& bFound)
{
	bFound = false;
	if (const UThirdwebRuntimeSettings* Settings = Get())
	{
		if (Settings->EngineSigners.Num() > 0)
		{
			bFound = true;
			return Settings->EngineSigners[0];
		}
	}
	return TEXT("");
}

FString UThirdwebRuntimeSettings::GetEncryptionKey()
{
	if (const UThirdwebRuntimeSettings* Settings = Get())
	{
		return Settings->EncryptionKey;
	}
	return TEXT("");
}

FString UThirdwebRuntimeSettings::GetStorageDirectory()
{
	FString StorageDir = FPaths::Combine(IFileManager::Get().ConvertToAbsolutePathForExternalAppForWrite(*FPaths::ProjectSavedDir()), "Thirdweb", "InAppWallet");
	TW_LOG(Verbose, TEXT("StorageDir::%s"), *StorageDir);
	return StorageDir;
}

bool UThirdwebRuntimeSettings::IsExternalOAuthBackend(const EThirdwebOAuthProvider Provider)
{
	if (const UThirdwebRuntimeSettings* Settings = Get())
	{
		if (Settings->bOverrideOAuthBrowserProviderBackends)
		{
			return static_cast<int>(StaticClass()->GetDefaultObject<UThirdwebRuntimeSettings>()->OAuthBrowserProviderBackendOverrides[static_cast<int>(Provider)]) == 1;
		}
		return static_cast<int>(Settings->OAuthBrowserProviderBackendOverrides[static_cast<int>(Provider)]) == 1;
	}
	return false;
}

FString UThirdwebRuntimeSettings::GetEcosystemId()
{
	if (const UThirdwebRuntimeSettings* Settings = Get())
	{
		return Settings->EcosystemId.TrimStartAndEnd();
	}
	return TEXT("");
}

FString UThirdwebRuntimeSettings::GetClientId()
{
	if (const UThirdwebRuntimeSettings* Settings = Get())
	{
		return Settings->ClientID.TrimStartAndEnd();
	}
	return TEXT("");
}

FString UThirdwebRuntimeSettings::GetBundleId()
{
	if (const UThirdwebRuntimeSettings* Settings = Get())
	{
		return Settings->BundleID.TrimStartAndEnd();
	}
	return TEXT("");
}

bool UThirdwebRuntimeSettings::AnalyticsEnabled()
{
	if (const UThirdwebRuntimeSettings* Settings = Get())
	{
		return Settings->bSendAnalytics;
	}
	return false;
}

FString UThirdwebRuntimeSettings::GetEngineBaseUrl()
{
	if (const UThirdwebRuntimeSettings* Settings = Get())
	{
		FString Url = Settings->EngineBaseUrl.TrimStartAndEnd();
		return Url.EndsWith("/") ? Url.LeftChop(1) : Url;
	}
	return TEXT("");
}

FString UThirdwebRuntimeSettings::GetEngineAccessToken()
{
	if (const UThirdwebRuntimeSettings* Settings = Get())
	{
		return Settings->EngineAccessToken.TrimStartAndEnd();
	}
	return TEXT("");
}
