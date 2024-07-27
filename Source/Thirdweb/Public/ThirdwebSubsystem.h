// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "HttpRouteHandle.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ThirdwebSubsystem.generated.h"

// OAuth Success Delegate
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOAuthSuccessDelegate, int64, WalletHandle, const FString&, Message);
// OAuth Failure Delegate
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOAuthFailureDelegate, int64, WalletHandle, const FString&, Message);

/**
 * Thirdweb Game Instance Subsystem
 */
UCLASS()
class THIRDWEB_API UThirdwebSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	// Overrides
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	// PRIVATE KEY WALLET FUNCTIONS

	// Blueprint callable function to create a private key wallet
	UFUNCTION(BlueprintCallable, Category = "Thirdweb|PrivateKeyWallet")
	void CreatePrivateKeyWallet(const FString& PrivateKey, bool& Success, bool& CanRetry, FString& Output);

	// Blueprint callable function to generate a private key wallet
	UFUNCTION(BlueprintCallable, Category = "Thirdweb|PrivateKeyWallet")
	void GeneratePrivateKeyWallet(bool& Success, bool& CanRetry, FString& Output);

	// IN-APP WALLET FUNCTIONS

	// Blueprint callable function to create an InAppWallet
	UFUNCTION(BlueprintCallable, Category = "Thirdweb|InAppWallet")
	void CreateInAppWallet(const FString& Email, const FString& OAuthMethod, bool& bSuccess, bool& CanRetry, FString& Output);

	// Blueprint callable function to send OTP
	UFUNCTION(BlueprintCallable, Category = "Thirdweb|InAppWallet")
	void SendOTP(int64 InAppWalletHandle, bool& Success, bool& CanRetry, FString& Output);

	// Blueprint callable function to verify OTP
	UFUNCTION(BlueprintCallable, Category = "Thirdweb|InAppWallet")
	void VerifyOTP(int64 InAppWalletHandle, const FString& OTP, bool& Success, bool& CanRetry, FString& Output);

	// Blueprint callable function to fetch OAuth login link
	UFUNCTION(BlueprintCallable, Category = "Thirdweb|InAppWallet")
	void FetchOAuthLoginLink(int64 InAppWalletHandle, const FString& RedirectUrl, bool& Success, bool& CanRetry, FString& Output);

	// Blueprint callable function to sign in with OAuth
	UFUNCTION(BlueprintCallable, Category = "Thirdweb|InAppWallet")
	void SignInWithOAuth(int64 InAppWalletHandle, const FString& AuthResult, bool& Success, bool& CanRetry, FString& Output);

	// Blueprint callable function to start OAuth login flow using default browser implementation
	UFUNCTION(BlueprintCallable, Category = "Thirdweb|InAppWallet")
	void LoginWithOauthDefault(int64 InAppWalletHandle);

	// SMART WALLET FUNCTIONS

	UFUNCTION(BlueprintCallable, Category = "Thirdweb|SmartWallet")
	void CreateSmartWallet(int64 PersonalWalletHandle,
	                       const FString& ChainID,
	                       bool Gasless,
	                       const FString& Factory,
	                       const FString& AccountOverride,
	                       bool& Success,
	                       bool& CanRetry,
	                       FString& Output);

	// Blueprint callable function to check if a smart wallet is deployed
	UFUNCTION(BlueprintCallable, Category = "Thirdweb|SmartWallet")
	void IsSmartWalletDeployed(int64 SmartWalletHandle, bool& Success, bool& CanRetry, FString& Output);

	// Blueprint callable function to get all admins of a smart wallet
	UFUNCTION(BlueprintCallable, Category = "Thirdweb|SmartWallet")
	void GetSmartWalletAdmins(int64 SmartWalletHandle, bool& Success, bool& CanRetry, FString& Output);

	// Blueprint callable function to get all active signers of a smart wallet
	UFUNCTION(BlueprintCallable, Category = "Thirdweb|SmartWallet")
	void GetSmartWalletActiveSigners(int64 SmartWalletHandle, bool& Success, bool& CanRetry, FString& Output);

	// Blueprint callable function to create a session key for a smart wallet
	UFUNCTION(BlueprintCallable, Category = "Thirdweb|SmartWallet")
	void CreateSmartWalletSessionKey(int64 SmartWalletHandle,
	                                 const FString& SignerAddress,
	                                 const FString& IsAdmin,
	                                 const TArray<FString>& ApprovedTargets,
	                                 const FString& NativeTokenLimitPerTransactionInWei,
	                                 const FString& PermissionStartTimestamp,
	                                 const FString& PermissionEndTimestamp,
	                                 const FString& ReqValidityStartTimestamp,
	                                 const FString& ReqValidityEndTimestamp,
	                                 bool& Success,
	                                 bool& CanRetry,
	                                 FString& Output);

	// THIRDWEB WALLET FUNCTIONS

	// Blueprint callable function to get the wallet address
	UFUNCTION(BlueprintCallable, Category = "Thirdweb|Wallets")
	void GetWalletAddress(int64 WalletHandle, bool& Success, bool& CanRetry, FString& Output);

	// Blueprint callable function to sign a message
	UFUNCTION(BlueprintCallable, Category = "Thirdweb|Wallets")
	void SignMessage(int64 WalletHandle, const FString& Message, bool& Success, bool& CanRetry, FString& Output);

	// Blueprint callable function to check if a wallet is connected
	UFUNCTION(BlueprintCallable, Category = "Thirdweb|Wallets")
	void IsConnected(int64 WalletHandle, bool& Success, bool& CanRetry, FString& Output);

	// Blueprint callable function to disconnect a wallet
	UFUNCTION(BlueprintCallable, Category = "Thirdweb|Wallets")
	void Disconnect(int64 WalletHandle, bool& Success, bool& CanRetry, FString& Output);

	// Static subsystem accessor convenience function for C++ implementations
	static UThirdwebSubsystem* Get(const UObject* WorldContextObject);
	
private:
	void CheckOAuthCompletion();

public:
	// Delegates

	UPROPERTY(BlueprintAssignable, Category = "Thirdweb|InAppWallet")
	FOAuthSuccessDelegate OnOAuthSuccess;

	UPROPERTY(BlueprintAssignable, Category = "Thirdweb|InAppWallet")
	FOAuthFailureDelegate OnOAuthFailure;

	// CLIENT CONFIGURATION
	// Properties for configurable parameters

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thirdweb")
	FString ClientID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thirdweb")
	FString BundleID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thirdweb")
	FString SecretKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thirdweb")
	FString StorageDirectoryPath;

private:
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
