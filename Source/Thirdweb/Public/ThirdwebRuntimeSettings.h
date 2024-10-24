// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "ThirdwebCommon.h"

#include "Engine/DeveloperSettings.h"

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
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category=Global)
	FString ClientID;

	/** Stores the bundle identifier. */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category=Global)
	FString BundleID;
	

	/** Required if using custom auth methods via standard InApp wallets (Non-Ecosystem) */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="Wallets|InApp")
	FString EncryptionKey;

	/** Ecosystem Wallet Identifier tied to your Thirdweb Ecosystem account. Only relevant when using Ecosystem Wallets. e.g. `ecosystem.my-cool-game` */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="Wallets|Ecosystem")
	FString EcosystemId;

	/** Enable if you plan to use varying partner ids. Disabling this will hide the input on relevant nodes */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, DisplayName="Use Partner IDs", Category="Wallets|Ecosystem")
	bool bUsePartnerIds = true;
	
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
	static TArray<FString> GetThirdwebGlobalEngineSigners();

	/** Gets the first global engine signer in the array, if any */
	UFUNCTION(BlueprintPure, Category="Thirdweb|Settings", meta=(ReturnDisplayName="Signers"))
	static FString GetThirdwebGlobalEngineSigner(bool& bFound);

	/** Static accessor to get EncryptionKey */
	UFUNCTION(BlueprintPure, Category="Thirdweb|Settings")
	static FString GetEncryptionKey();
	
	/** Static accessor to retrieve the absolute path of the thirdweb InAppWallet platform */
	static FString GetStorageDirectory();

	/** Static accessor to get the resolved backend of an OAuth provider */
	static bool IsExternalOAuthBackend(const EThirdwebOAuthProvider Provider);

	/** Static accessor to get EcosystemId */
	static FString GetEcosystemId();

	/** Static accessor to get bUsePartnerIds */
	static bool ShowPartnerIds();

	/** Static accessor to check EcosystemId validity */
	static bool IsEcosystem() { return !GetEcosystemId().IsEmpty(); }

	/** Static accessor to get ClientId */
	static FString GetClientId();
	
	/** Static accessor to get BundleId */
	static FString GetBundleId();
	
	/** Static accessor to check Analytics Opt-In status */
	static bool AnalyticsEnabled();

	/** Static accessor to get BaseEngineUrl */
	static FString GetEngineBaseUrl();
	
	/** Static accessor to get AccessToken */
	static FString GetEngineAccessToken();
};
