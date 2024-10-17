// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "Thirdweb.h"
#include "ThirdwebCommon.h"
#include "ThirdwebLog.h"

#include "Engine/DeveloperSettings.h"

#include "HAL/FileManager.h"

#include "Misc/Paths.h"

#include "ThirdwebRuntimeSettings.generated.h"

enum class EThirdwebOAuthBrowserBackend : uint8;
enum class EThirdwebOAuthProvider : uint8;
enum class EThirdwebAuthenticationMethod : uint8;

/**
 * This class acts as a configuration holder to manage various settings required for the runtime environment in the Thirdweb system.
 */
UCLASS(Config=Engine, DefaultConfig, meta=(DisplayName="Thirdweb"))
class THIRDWEB_API UThirdwebRuntimeSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UThirdwebRuntimeSettings();
	

	/** Stores the client identifier. */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category=Config)
	FString ClientID;

	/** Stores the bundle identifier. */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category=Config)
	FString BundleID;
	

	/** Required if using custom auth methods via standard InApp wallets (Non-Ecosystem) */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="Wallets|InApp")
	FString EncryptionKey;

	/** Ecosystem Wallet Identifier tied to your Thirdweb Ecosystem account. Only relevant when using Ecosystem Wallets. e.g. `ecosystem.my-cool-game` */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="Wallets|Ecosystem")
	FString EcosystemId;
	
	/** Optional array of engine signers stored globally for convenience */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="Wallets|Smart")
	TArray<FString> EngineSigners;

	/** Publicly accessible Base URL To your Engine instance. Needed for all Engine nodes */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, DisplayName="Base URL", Category=Engine)
	FString EngineBaseUrl;

	/** Access Token for Engine Authorization */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, DisplayName="Access Token", Category=Engine)
	FString EngineAccessToken;
	
	/** Opt in or out of connect analytics */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category=Advanced)
	bool bSendAnalytics;

	/** Edit Condition for overriding OAuth Browser Provider Backends */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category=Advanced, meta=(InlineEditConditionToggle))
	bool bOverrideOAuthBrowserProviderBackends;

	UPROPERTY(Config, EditAnywhere, Category=Advanced, meta=(EditCondition="bOverrideOAuthBrowserProviderBackends", ArraySizeEnum="EThirdwebOAuthProvider"))
	EThirdwebOAuthBrowserBackend OAuthBrowserProviderBackendOverrides[static_cast<int>(EThirdwebOAuthProvider::None)];
	
	UFUNCTION(BlueprintPure, Category="Thirdweb", DisplayName="Get Thirdweb Runtime Settings")
	static const UThirdwebRuntimeSettings* Get() { return GetDefault<UThirdwebRuntimeSettings>(); }

	UFUNCTION(CallInEditor, Category=Encryption)
	void GenerateEncryptionKey();

	UFUNCTION(BlueprintPure, Category="Thirdweb|Settings")
	static TArray<FString> GetThirdwebGlobalEngineSigners()
	{
		if (const UThirdwebRuntimeSettings* Settings = Get())
		{
			return Settings->EngineSigners;
		}
		return {};
	}

	/** Gets the first global engine signer in the array, if any */
	UFUNCTION(BlueprintPure, Category="Thirdweb|Settings", meta=(ReturnDisplayName="Signers"))
	static FString GetThirdwebGlobalEngineSigner(bool& bFound)
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

	/** Static accessor to get EncryptionKey */
	UFUNCTION(BlueprintPure, Category="Thirdweb|Settings")
	static FString GetEncryptionKey()
	{
		if (const UThirdwebRuntimeSettings* Settings = Get())
		{
			return Settings->EncryptionKey;
		}
		return TEXT("");
	}
	
	/** Static accessor to retrieve the absolute path of the thirdweb InAppWallet platform */
	static FString GetStorageDirectory()
	{
		FString StorageDir = FPaths::Combine(IFileManager::Get().ConvertToAbsolutePathForExternalAppForWrite(*FPaths::ProjectSavedDir()), "Thirdweb", "InAppWallet");
		TW_LOG(Verbose, TEXT("StorageDir::%s"), *StorageDir)
		return StorageDir;
	}

	/** Static accessor to get the resolved backend of an OAuth provider */
	static bool IsExternalOAuthBackend(const EThirdwebOAuthProvider Provider)
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

	/** Static accessor to get EcosystemId */
	static FString GetEcosystemId()
	{
		if (const UThirdwebRuntimeSettings* Settings = Get())
		{
			return Settings->EcosystemId.TrimStartAndEnd();
		}
		return TEXT("");
	}

	/** Static accessor to check EcosystemId validity */
	static bool IsEcosystem() { return !GetEcosystemId().IsEmpty(); }

	/** Static accessor to get ClientId */
	static FString GetClientId()
	{
		if (const UThirdwebRuntimeSettings* Settings = Get())
		{
			return Settings->ClientID.TrimStartAndEnd();
		}
		return TEXT("");
	}
	
	/** Static accessor to get BundleId */
	static FString GetBundleId()
	{
		if (const UThirdwebRuntimeSettings* Settings = Get())
		{
			return Settings->BundleID.TrimStartAndEnd();
		}
		return TEXT("");
	}
	
	/** Static accessor to check Analytics Opt-In status */
	static bool AnalyticsEnabled()
	{
		if (const UThirdwebRuntimeSettings* Settings = Get())
		{
			return Settings->bSendAnalytics;
		}
		return false;
	}

	/** Static accessor to get BaseEngineUrl */
	static FString GetEngineBaseUrl()
	{
		if (const UThirdwebRuntimeSettings* Settings = Get())
		{
			FString Url = Settings->EngineBaseUrl.TrimStartAndEnd();
			return Url.EndsWith("/") ? Url.LeftChop(1) : Url;
		}
		return TEXT("");
	}
	
	/** Static accessor to get AccessToken */
	static FString GetEngineAccessToken()
	{
		if (const UThirdwebRuntimeSettings* Settings = Get())
		{
			return Settings->EngineAccessToken.TrimStartAndEnd();
		}
		return TEXT("");
	}
};
