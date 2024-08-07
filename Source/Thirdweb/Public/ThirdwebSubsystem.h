// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "HttpRouteHandle.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ThirdwebSubsystem.generated.h"

enum class EThirdwebOAuthProvider : uint8;
enum class ESmartWalletDeployedFunctionResult : uint8;
enum class EOTPVerificationFunctionResult : uint8;
enum class EFunctionResult : uint8;
struct FWalletHandle;
class UThirdwebRuntimeSettings;

DECLARE_DELEGATE_OneParam(FOauthResponseDelegate, FString)

/**
 * Thirdweb Game Instance Subsystem
 */
UCLASS(meta=(DisplayName="Thirdweb"))
class THIRDWEB_API UThirdwebSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	// Overrides
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	// IN-APP WALLET FUNCTIONS

	// Blueprint callable function to create an InAppWallet
	UFUNCTION(BlueprintCallable, Category="Thirdweb|Wallets|In App", DisplayName="Create Email Wallet", meta=(ExpandEnumAsExecs="ReturnValue"))
	EFunctionResult BP_CreateInAppEmailWallet(const FString& Email, FWalletHandle& Wallet, FString& Error);
	bool CreateInAppEmailWallet(const FString& Email, FWalletHandle& Wallet, FString& Error);
	
	UFUNCTION(BlueprintCallable, Category="Thirdweb|Wallets|In App", DisplayName="Create OAuth Wallet", meta=(ExpandEnumAsExecs="ReturnValue"))
	EFunctionResult BP_CreateInAppOAuthWallet(const EThirdwebOAuthProvider Provider, FWalletHandle& Wallet, FString& Error);
	bool CreateInAppOAuthWallet(const EThirdwebOAuthProvider Provider, FWalletHandle& Wallet, FString& Error);
	
	// SMART WALLET FUNCTIONS

	UFUNCTION(BlueprintCallable, Category="Thirdweb|Wallets|Smart Wallet", DisplayName="Create Smart Wallet", meta=(ExpandEnumAsExecs="ReturnValue", AdvancedDisplay="bGasless,Factory,AccountOverride"))
	EFunctionResult BP_CreateSmartWallet(const FWalletHandle& PersonalWallet, FWalletHandle& SmartWallet, FString& Error, const int64 ChainID, const bool bGasless = true, const FString& Factory = "", const FString& AccountOverride = "");
	bool CreateSmartWallet(const FWalletHandle& PersonalWallet, const int64 ChainID, const bool bGasless, const FString& Factory, const FString& AccountOverride, FWalletHandle& SmartWallet, FString& Error);

	// Static subsystem accessor convenience function for C++ implementations
	static UThirdwebSubsystem* Get(const UObject* WorldContextObject);

private:
	UPROPERTY(Transient)
	const UThirdwebRuntimeSettings* Settings;
	
	FEvent* AuthEvent;
	bool bAuthComplete;
	FString OAuthResult;
	int64 OAuthWalletHandle;
	FHttpRouteHandle RouteHandle;
	FString OAuthLoginUrl;
	FString OAuthOutputString;
	bool bOAuthSuccess;
	bool bOAuthCanRetry;
};
