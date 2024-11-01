// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "ThirdwebRuntimeSettings.h"

#include "ThirdwebCommon.h"
#include "ThirdwebLog.h"
#include "ThirdwebUtils.h"

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

	const TArray Trimmable = {
		GET_MEMBER_NAME_CHECKED(UThirdwebRuntimeSettings, ClientId),
		GET_MEMBER_NAME_CHECKED(UThirdwebRuntimeSettings, BundleId),
		GET_MEMBER_NAME_CHECKED(UThirdwebRuntimeSettings, EncryptionKey),
		GET_MEMBER_NAME_CHECKED(UThirdwebRuntimeSettings, EcosystemId),
		GET_MEMBER_NAME_CHECKED(UThirdwebRuntimeSettings, PartnerId),
		GET_MEMBER_NAME_CHECKED(UThirdwebRuntimeSettings, EngineBaseUrl),
		GET_MEMBER_NAME_CHECKED(UThirdwebRuntimeSettings, EngineAccessToken),
		GET_MEMBER_NAME_CHECKED(UThirdwebRuntimeSettings, CustomAppUri)
	};

	bool bChanged = false;
	// ReSharper disable once CppTooWideScopeInitStatement
	const FName CurrentPropertyName = PropertyChangedEvent.GetMemberPropertyName();
	if (CurrentPropertyName == GET_MEMBER_NAME_CHECKED(UThirdwebRuntimeSettings, OAuthBrowserProviderBackendOverrides))
	{
		for (const EThirdwebOAuthProvider Provider : ExternalOnlyProviders)
		{
			if (OAuthBrowserProviderBackendOverrides[static_cast<int>(Provider)] != EThirdwebOAuthBrowserBackend::External)
			{
				OAuthBrowserProviderBackendOverrides[static_cast<int>(Provider)] = EThirdwebOAuthBrowserBackend::External;
				bChanged = true;
			}
		}
	}
	if (Trimmable.Contains(CurrentPropertyName))
	{
		FString Value;
		PropertyChangedEvent.Property->GetValue_InContainer(this, &Value);
		if (const FString Trimmed = Value.TrimStartAndEnd(); !Trimmed.Equals(Value))
		{
			PropertyChangedEvent.Property->SetValue_InContainer(this, &Trimmed);
			bChanged = true;
		}
	}
	if (CurrentPropertyName == GET_MEMBER_NAME_CHECKED(UThirdwebRuntimeSettings, EngineBaseUrl))
	{
		FString Value;
		PropertyChangedEvent.Property->GetValue_InContainer(this, &Value);
		if (Value.EndsWith("/"))
		{
			const FString NewValue = Value.LeftChop(1);
			PropertyChangedEvent.Property->SetValue_InContainer(this, &NewValue);
		}
	}
	if (CurrentPropertyName == GET_MEMBER_NAME_CHECKED(UThirdwebRuntimeSettings, EngineSigners))
	{
		bool bArrayChanged = false;
		TArray<FString> Values;
		PropertyChangedEvent.Property->GetValue_InContainer(this, &Values);
		for (int32 i = 0; i < Values.Num(); i++)
		{
			if (FString NewValue = Values[i].TrimStartAndEnd(); !NewValue.Equals(Values[i]))
			{
				Values[i] = NewValue;
				bArrayChanged = true;
			}
		}
		if (bArrayChanged)
		{
			PropertyChangedEvent.Property->SetValue_InContainer(this, &Values);
			bChanged = true;
		}
	}
	if (bChanged && MarkPackageDirty())
	{
		PostEditChange();
	}
}

bool UThirdwebRuntimeSettings::CanEditChange(const FProperty* InProperty) const
{
	if (!Super::CanEditChange(InProperty))
	{
		return false;
	}

	// ReSharper disable once CppTooWideScopeInitStatement
	const FName PropertyName = InProperty->GetFName();
	if (PropertyName == GET_MEMBER_NAME_CHECKED(UThirdwebRuntimeSettings, PartnerId))
	{
		return IsEcosystem();
	}
	return true;
}
#endif

void UThirdwebRuntimeSettings::GenerateEncryptionKey()
{
#if WITH_EDITOR
	EncryptionKey = ThirdwebUtils::Internal::GenerateUUID();
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
		return Get()->PartnerId.TrimStartAndEnd();
	}
	return TEXT("");
}

bool UThirdwebRuntimeSettings::IsEcosystem()
{
	const FString ID = GetEcosystemId();
	return !ID.IsEmpty() && ID.StartsWith(TEXT("ecosystem."));
}

FString UThirdwebRuntimeSettings::GetClientId()
{
	if (const UThirdwebRuntimeSettings* Settings = Get())
	{
		return Settings->ClientId.TrimStartAndEnd();
	}
	return TEXT("");
}

FString UThirdwebRuntimeSettings::GetBundleId()
{
	if (const UThirdwebRuntimeSettings* Settings = Get())
	{
		return Settings->BundleId.TrimStartAndEnd();
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
