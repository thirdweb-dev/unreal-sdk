// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "ThirdwebCommon.h"
#include "Engine/DeveloperSettings.h"
#include "ThirdwebRuntimeSettings.generated.h"

enum class EThirdwebOAuthProvider : uint8;
enum class EThirdwebAuthenticationMethod : uint8;

/**
 * This class acts as a configuration holder to manage various settings required for the runtime environment in the Thirdweb system.
 */
UCLASS(Config = Engine, DefaultConfig, meta = (DisplayName = "Thirdweb"))
class THIRDWEB_API UThirdwebRuntimeSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UThirdwebRuntimeSettings();

#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent &PropertyChangedEvent) override;
	virtual bool CanEditChange(const FProperty *InProperty) const override;
#endif

protected:
	/** Stores the client identifier. */
	UPROPERTY(Config, EditAnywhere, DisplayName = "Client ID", Category = Global)
	FString ClientId;

	/** Stores the bundle identifier. */
	UPROPERTY(Config, EditAnywhere, DisplayName = "Bundle ID", Category = Global)
	FString BundleId;

	/** Ecosystem Wallet Identifier tied to your Thirdweb Ecosystem account. Only relevant when using Ecosystem Wallets. e.g. `ecosystem.my-cool-game` */
	UPROPERTY(Config, EditAnywhere, DisplayName = "Ecosystem ID", Category = "Wallets|Ecosystem")
	FString EcosystemId;

	/** Ecosystem Wallet Partner ID - Needed if Allowlist-based Ecosystem */
	UPROPERTY(Config, EditAnywhere, DisplayName = "Partner ID", Category = "Wallets|Ecosystem")
	FString PartnerId;

	/** Required if using custom auth methods via standard InApp wallets (Non-Ecosystem) */
	UPROPERTY(Config, EditAnywhere, DisplayName = "Encrypton Key (Legacy)", Category = "Wallets|Ecosystem|Advanced")
	FString EncryptionKey;

	/** Optional array of engine signers stored globally for convenience */
	UPROPERTY(Config, EditAnywhere, Category = "Wallets|Smart")
	TArray<FString> EngineSigners;

	/** Publicly accessible Base URL To your Engine instance. Needed for all Engine nodes */
	UPROPERTY(Config, EditAnywhere, DisplayName = "Base URL", Category = Engine)
	FString EngineBaseUrl;

	/** Access Token for Engine Authorization */
	UPROPERTY(Config, EditAnywhere, DisplayName = "Access Token", meta = (ConfigHierarchyEditable), Category = Engine)
	FString EngineAccessToken;

	/** Opt in or out of connect analytics */
	UPROPERTY(Config, EditAnywhere, Category = Advanced)
	bool bSendAnalytics;

	/** Edit Condition for overriding Custom Application Schema */
	UPROPERTY(Config, EditAnywhere, DisplayName = "Override App URI", meta = (InlineEditConditionToggle, ConfigHierarchyEditable), Category = "Advanced|Wallets")
	bool bOverrideAppUri;

	/** Custom Application URI for oauth redirects. default is {bundleId}://{clientId} */
	UPROPERTY(Config, EditAnywhere, DisplayName = "Custom App URI", meta = (EditCondition = "bOverrideAppUri", ConfigHierarchyEditable), Category = "Advanced|Wallets")
	FString CustomAppUri;

	/** Edit Condition for overriding the final page shown after auth is complete */
	UPROPERTY(Config, EditAnywhere, meta = (InlineEditConditionToggle, ConfigHierarchyEditable), Category = "Advanced|Wallets")
	bool bOverrideExternalAuthRedirectUri;

	/** Custom URI shown after external auth is complete */
	UPROPERTY(Config, EditAnywhere, DisplayName = "Custom External Auth Redirect URI", meta = (EditCondition = "bOverrideExternalAuthRedirectUri", ConfigHierarchyEditable), Category = "Advanced|Wallets")
	FString CustomExternalAuthRedirectUri;

private:
public:
	// UFUNCTION(CallInEditor, Category="Wallets|InApp")
	void GenerateEncryptionKey();

	UFUNCTION(BlueprintPure, Category = "Thirdweb|Settings")
	static TArray<FString> GetEngineSigners();

	/** Gets the first engine signer in the array, if any */
	UFUNCTION(BlueprintPure, Category = "Thirdweb|Settings", meta = (ReturnDisplayName = "Signer"))
	static FString GetEngineSigner();

	/** Static accessor to get EncryptionKey */
	UFUNCTION(BlueprintPure, DisplayName = "Get External Auth Redirect URI", Category = "Thirdweb|Settings")
	static FString GetExternalAuthRedirectUri();

	/** Static accessor to get EncryptionKey */
	UFUNCTION(BlueprintPure, Category = "Thirdweb|Settings")
	static FString GetEncryptionKey();

	/** Static accessor to retrieve the absolute path of the thirdweb InAppWallet platform */
	static FString GetStorageDirectory();

	/** Static accessor to get EcosystemId */
	static FString GetEcosystemId();

	/** Static accessor to get PartnerId */
	static FString GetPartnerId();

	/** Static accessor to check EcosystemId validity */
	UFUNCTION(BlueprintPure, Category = "Thirdweb|Settings")
	static bool IsEcosystem();

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

	/** Static accessor for AppUri */
	static FString GetAppUri();

	/** Convenience Getter */
	UFUNCTION(BlueprintPure, DisplayName = "Get Thirdweb Runtime Settings", meta = (ReturnDisplayName = "Settings"), Category = "Thirdweb|Settings")
	static const UThirdwebRuntimeSettings *Get() { return GetDefault<UThirdwebRuntimeSettings>(); }

protected:
	// UFUNCTION(CallInEditor, Category="Wallets|Smart")
	void FetchEngineSigners();

private:
	static const FString DefaultExternalAuthRedirectUri;
};
