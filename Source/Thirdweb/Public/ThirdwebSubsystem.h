// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "ThirdwebCommon.h"
#include "HttpRouteHandle.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ThirdwebSubsystem.generated.h"

DECLARE_DELEGATE_OneParam(FOauthResponseDelegate, FString, Message)

struct FWalletHandle;

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

	// PRIVATE KEY WALLET FUNCTIONS

	// Blueprint function to create a private key wallet
	UFUNCTION(BlueprintCallable, Category="Thirdweb|PrivateKeyWallet", DisplayName="Create Private Key Wallet", meta=(ExpandEnumAsExecs="ReturnValue"))
	EFunctionResult BP_CreatePrivateKeyWallet(const FString& PrivateKey, FWalletHandle& Wallet, FString& Error);
	bool CreatePrivateKeyWallet(const FString& PrivateKey, bool& bCanRetry, FWalletHandle& Wallet, FString& Error);

	// Blueprint callable function to generate a private key wallet
	UFUNCTION(BlueprintCallable, Category="Thirdweb|PrivateKeyWallet", DisplayName="Generate Private Key Wallet", meta=(ExpandEnumAsExecs="ReturnValue"))
	EFunctionResult BP_GeneratePrivateKeyWallet(FWalletHandle& Wallet, FString& Error);
	bool GeneratePrivateKeyWallet(FWalletHandle& Wallet, bool& CanRetry, FString& Error);

	// IN-APP WALLET FUNCTIONS

	// Blueprint callable function to create an InAppWallet
	UFUNCTION(BlueprintCallable, Category="Thirdweb|InAppWallet", DisplayName="Create In App Email Wallet", meta=(ExpandEnumAsExecs="ReturnValue"))
	EFunctionResult BP_CreateInAppEmailWallet(const FString& Email, FWalletHandle& Wallet, FString& Error);

	UFUNCTION(BlueprintCallable, Category="Thirdweb|InAppWallet", DisplayName="Create In App OAuth Wallet", meta=(ExpandEnumAsExecs="ReturnValue"))
	EFunctionResult BP_CreateInAppOAuthWallet(const FString& OAuthMethod, FWalletHandle& Wallet, FString& Error);
	
	bool CreateInAppWallet(const FString& Email, const FString& OAuthMethod, bool& bCanRetry, FWalletHandle& Wallet, FString& Error);
	
	// Blueprint callable function to send OTP
	UFUNCTION(BlueprintCallable, Category="Thirdweb|InAppWallet", DisplayName="Send OTP", meta=(ExpandEnumAsExecs="ReturnValue"))
	EFunctionResult BP_SendOTP(const FWalletHandle& Wallet, FString& Error);
	bool SendOTP(const FWalletHandle& Wallet, bool& CanRetry, FString& Error);

	// Blueprint callable function to verify OTP
	UFUNCTION(BlueprintCallable, Category="Thirdweb|InAppWallet", DisplayName="Verify OTP", meta=(ExpandEnumAsExecs="ReturnValue"))
	EFunctionResult BP_VerifyOTP(const FWalletHandle& Wallet, const FString& OTP, FString& Error);
	bool VerifyOTP(const FWalletHandle& Wallet, const FString& OTP, bool& CanRetry, FString& Error);

	// Blueprint callable function to fetch OAuth login link
	UFUNCTION(BlueprintCallable, Category="Thirdweb|InAppWallet", DisplayName="Fetch OAuth Login Link", meta=(ExpandEnumAsExecs="ReturnValue"))
	EFunctionResult BP_FetchOAuthLoginLink(const FWalletHandle& Wallet, const FString& RedirectUrl, FString& LoginLink, FString& Error);
	bool FetchOAuthLoginLink(const FWalletHandle& Wallet, const FString& RedirectUrl, bool& CanRetry, FString& LoginLink, FString& Error);

	// Blueprint callable function to sign in with OAuth
	UFUNCTION(BlueprintCallable, Category="Thirdweb|InAppWallet", DisplayName="Sign In With OAuth", meta=(ExpandEnumAsExecs="ReturnValue"))
	EFunctionResult BP_SignInWithOAuth(const FWalletHandle& Wallet, const FString& AuthResult, FString& Output);
	bool SignInWithOAuth(const FWalletHandle& Wallet, const FString& AuthResult, bool& CanRetry, FString& Output);
	
	void LoginWithOAuthDefault(const FWalletHandle& Wallet, const FOauthResponseDelegate& SuccessDelegate, const FOauthResponseDelegate& ErrorDelegate);

	// SMART WALLET FUNCTIONS

	UFUNCTION(BlueprintCallable, Category="Thirdweb|SmartWallet", DisplayName="Create Smart Wallet", meta=(ExpandEnumAsExecs="ReturnValue", AdvancedDisplay="bGasless,Factory,AccountOverride"))
	EFunctionResult BP_CreateSmartWallet(const FWalletHandle& PersonalWallet, const FString& ChainID, const bool bGasless, const FString& Factory, const FString& AccountOverride,
	                                     FWalletHandle& Wallet, FString& Error);
	bool CreateSmartWallet(const FWalletHandle& PersonalWallet, const FString& ChainID, bool bGasless, const FString& Factory, const FString& AccountOverride, FWalletHandle& Wallet, bool& CanRetry,
	                       FString& Error);

	// Blueprint callable function to check if a smart wallet is deployed
	UFUNCTION(BlueprintCallable, Category="Thirdweb|SmartWallet", DisplayName="Is Smart Wallet Deployed", meta=(ExpandEnumAsExecs="ReturnValue"))
	ESmartWalletDeployedFunctionResult BP_IsSmartWalletDeployed(const FWalletHandle& Wallet, FString& Error);
	bool IsSmartWalletDeployed(const FWalletHandle& Wallet, bool& bDeployed, bool& CanRetry, FString& Error);

	// Blueprint callable function to get all admins of a smart wallet
	UFUNCTION(BlueprintCallable, Category="Thirdweb|SmartWallet", DisplayName="Get Smart Wallet Admins", meta=(ExpandEnumAsExecs="ReturnValue"))
	EFunctionResult BP_GetSmartWalletAdmins(const FWalletHandle& Wallet, FString& Output);
	bool GetSmartWalletAdmins(const FWalletHandle& Wallet, bool& CanRetry, FString& Output);

	// Blueprint callable function to get all active signers of a smart wallet
	UFUNCTION(BlueprintCallable, Category="Thirdweb|SmartWallet", DisplayName="Get Smart Wallet Active Signers", meta=(ExpandEnumAsExecs="ReturnValue"))
	EFunctionResult BP_GetSmartWalletActiveSigners(const FWalletHandle& Wallet, FString& Output);
	bool GetSmartWalletActiveSigners(const FWalletHandle& Wallet, bool& CanRetry, FString& Output);

	// Blueprint callable function to create a session key for a smart wallet
	UFUNCTION(BlueprintCallable, Category="Thirdweb|SmartWallet", DisplayName="Create Smart Wallet Session Key",
		meta=(ExpandEnumAsExecs="ReturnValue", AutoCreateRefTerm="PermissionStart,PermissionEnd,RequestValidityStart,RequestValidityEnd"))
	EFunctionResult BP_CreateSmartWalletSessionKey(const FWalletHandle& Wallet, const FString& SignerAddress, const bool IsAdmin, const TArray<FString>& ApprovedTargets,
	                                               const FString& NativeTokenLimitPerTransactionInWei, const FDateTime& PermissionStart, const FDateTime& PermissionEnd,
	                                               const FDateTime& RequestValidityStart, const FDateTime& RequestValidityEnd, FString& Key, FString& Error);
	bool CreateSmartWalletSessionKey(const FWalletHandle& Wallet, const FString& SignerAddress, const bool bIsAdmin, const TArray<FString>& ApprovedTargets,
	                                 const FString& NativeTokenLimitPerTransactionInWei, const FDateTime& PermissionStart, const FDateTime& PermissionEnd, const FDateTime& RequestValidityStart,
	                                 const FDateTime& RequestValidityEnd, bool& CanRetry, FString& Key, FString& Error);

	// THIRDWEB WALLET FUNCTIONS

	// Blueprint callable function to get the wallet address
	UFUNCTION(BlueprintCallable, Category="Thirdweb|Wallets", DisplayName="Get Wallet Address", meta=(ExpandEnumAsExecs="ReturnValue"))
	EFunctionResult BP_GetWalletAddress(const FWalletHandle& Wallet, FString& Address, FString& Error);
	bool GetWalletAddress(const FWalletHandle& Wallet, bool& CanRetry, FString& Address, FString& Error);

	// Blueprint callable function to sign a message
	UFUNCTION(BlueprintCallable, Category="Thirdweb|Wallets", DisplayName="Sign Message", meta=(ExpandEnumAsExecs="ReturnValue"))
	EFunctionResult BP_SignMessage(const FWalletHandle& Wallet, const FString& Message, FString& Result, FString& Error);
	bool SignMessage(const FWalletHandle& Wallet, const FString& Message, bool& CanRetry, FString& Result, FString& Error);

	// Blueprint callable function to check if a wallet is connected
	UFUNCTION(BlueprintCallable, Category="Thirdweb|Wallets", DisplayName="Is Connected", meta=(ExpandEnumAsExecs="ReturnValue"))
	EWalletConnectedFunctionResult BP_IsConnected(const FWalletHandle& Wallet, FString& Error);
	bool IsConnected(const FWalletHandle& Wallet, bool& bIsConnected, bool& CanRetry, FString& Error);

	// Blueprint callable function to disconnect a wallet
	UFUNCTION(BlueprintCallable, Category="Thirdweb|Wallets", DisplayName="Disconnect", meta=(ExpandEnumAsExecs="ReturnValue"))
	EFunctionResult BP_Disconnect(const FWalletHandle& Wallet, FString& Error);
	bool Disconnect(const FWalletHandle& Wallet, bool& CanRetry, FString& Error);

	// Static subsystem accessor convenience function for C++ implementations
	static UThirdwebSubsystem* Get(const UObject* WorldContextObject);

private:
	void CheckOAuthCompletion();

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
