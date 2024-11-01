// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "ThirdwebRuntimeSettings.h"

#include "ThirdwebCommon.h"
#include "ThirdwebInternal.h"
#include "ThirdwebLog.h"

#include "HAL/FileManager.h"

#include "Misc/Paths.h"

// These Providers do not work with an embedded browser for various reasons:
// - Google: https://accounts.youtube.com/accounts/SetSID does not load
// - Facebook: https://www.facebook.com/privacy/consent/gdp does not load
// - Telegram: Forced transient pop-up window
const TArray<EThirdwebOAuthProvider> UThirdwebRuntimeSettings::ExternalOnlyProviders = {
	EThirdwebOAuthProvider::Google,
	EThirdwebOAuthProvider::Facebook,
	EThirdwebOAuthProvider::Telegram,
};

UThirdwebRuntimeSettings::UThirdwebRuntimeSettings()
{
	bSendAnalytics = true;
	bOverrideOAuthBrowserProviderBackends = false;
	bOverrideAppUri = false;
	for (const EThirdwebOAuthProvider Provider : ExternalOnlyProviders) OAuthBrowserProviderBackendOverrides[static_cast<int>(Provider)] = EThirdwebOAuthBrowserBackend::External;
}

#if WITH_EDITOR
void UThirdwebRuntimeSettings::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	// ReSharper disable once CppTooWideScopeInitStatement
	FName CurrentPropertyName = PropertyChangedEvent.GetMemberPropertyName();
	if (CurrentPropertyName == FName(TEXT("OAuthBrowserProviderBackendOverrides")))
	{
		bool bChanged = false;
		for (const EThirdwebOAuthProvider Provider : ExternalOnlyProviders)
		{
			if (OAuthBrowserProviderBackendOverrides[static_cast<int>(Provider)] != EThirdwebOAuthBrowserBackend::External)
			{
				OAuthBrowserProviderBackendOverrides[static_cast<int>(Provider)] = EThirdwebOAuthBrowserBackend::External;
				bChanged = true;
				
			}
		}
		if (bChanged)
		{
			if (MarkPackageDirty())
			{
				PostEditChange();
			}
		}
	}
}
#endif

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
	if (ExternalOnlyProviders.Contains(Provider)) return true;
#if PLATFORM_APPLE
	// Apple natively handles apple auth links, so we want to enforce that flow
	if (Provider == EThirdwebOAuthProvider::Apple) return true;
#endif

	if (const UThirdwebRuntimeSettings* Settings = Get())
	{
		if (Settings->bOverrideOAuthBrowserProviderBackends)
		{
			return static_cast<int>(Settings->OAuthBrowserProviderBackendOverrides[static_cast<int>(Provider)]) == 1;
		}
		return static_cast<int>(StaticClass()->GetDefaultObject<UThirdwebRuntimeSettings>()->OAuthBrowserProviderBackendOverrides[static_cast<int>(Provider)]) == 1;
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

FString UThirdwebRuntimeSettings::GetPartnerId()
{
	if (IsEcosystem())
	{
		return Get()->PartnerID.TrimStartAndEnd();
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

FString UThirdwebRuntimeSettings::GetAppUri()
{
	if (const UThirdwebRuntimeSettings* Settings = Get())
	{
		if (Settings->bOverrideAppUri)
		{
			
			return Settings->CustomAppUri.TrimStartAndEnd();
		}
		return FString::Printf(TEXT("bundleid://%s"), *GetBundleId());
	}
	return TEXT("");
}
