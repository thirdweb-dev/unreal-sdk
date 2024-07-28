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
	EFunctionResult BP_CreateSmartWallet(const FWalletHandle& LocalWallet, FWalletHandle& SmartWallet, FString& Error, const int64 ChainID, const bool bGasless = true, const FString& Factory = "", const FString& AccountOverride = "");
	bool CreateSmartWallet(const FWalletHandle& LocalWallet, const int64 ChainID, bool bGasless, const FString& Factory, const FString& AccountOverride, FWalletHandle& SmartWallet, FString& Error);

	// Blueprint callable function to get all admins of a smart wallet
	UFUNCTION(BlueprintCallable, Category="Thirdweb|Wallets|Smart Wallet", DisplayName="Get Smart Wallet Admins", meta=(ExpandEnumAsExecs="ReturnValue"))
	EFunctionResult BP_GetSmartWalletAdmins(const FWalletHandle& Wallet, FString& Output, FString& Error);
	bool GetSmartWalletAdmins(const FWalletHandle& Wallet, FString& Output, FString& Error);

	// Blueprint callable function to get all active signers of a smart wallet
	UFUNCTION(BlueprintCallable, Category="Thirdweb|Wallets|Smart Wallet", DisplayName="Get Smart Wallet Active Signers", meta=(ExpandEnumAsExecs="ReturnValue"))
	EFunctionResult BP_GetSmartWalletActiveSigners(const FWalletHandle& Wallet, FString& Output, FString& Error);
	bool GetSmartWalletActiveSigners(const FWalletHandle& Wallet, FString& Output, FString& Error);

	// Blueprint callable function to create a session key for a smart wallet
	UFUNCTION(BlueprintCallable, Category="Thirdweb|Wallets|Smart Wallet", DisplayName="Create Smart Wallet Session Key",
		meta=(ExpandEnumAsExecs="ReturnValue", AutoCreateRefTerm="PermissionStart,PermissionEnd,RequestValidityStart,RequestValidityEnd"))
	EFunctionResult BP_CreateSmartWalletSessionKey(const FWalletHandle& Wallet, const FString& SignerAddress, const bool IsAdmin, const TArray<FString>& ApprovedTargets,
	                                               const FString& NativeTokenLimitPerTransactionInWei, const FDateTime& PermissionStart, const FDateTime& PermissionEnd,
	                                               const FDateTime& RequestValidityStart, const FDateTime& RequestValidityEnd, FString& Key, FString& Error);
	bool CreateSmartWalletSessionKey(const FWalletHandle& Wallet, const FString& SignerAddress, const bool bIsAdmin, const TArray<FString>& ApprovedTargets,
	                                 const FString& NativeTokenLimitPerTransactionInWei, const FDateTime& PermissionStart, const FDateTime& PermissionEnd, const FDateTime& RequestValidityStart,
	                                 const FDateTime& RequestValidityEnd, FString& Key, FString& Error);


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
