// Copyright 2024 Cajun Pro LLC. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ThirdwebWalletHandle.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ThirdwebFunctionLibrary.generated.h"

enum class EFunctionResult : uint8;
enum class EThirdwebOAuthProvider : uint8;
enum class EOTPVerificationFunctionResult : uint8;
enum class ESmartWalletDeployedFunctionResult : uint8;
struct FWalletHandle;

/**
 * Thirdweb Function Library
 */
UCLASS(meta=(BlueprintThreadSafe, DisplayName="Thirdweb", ScriptName="ThirdwebFunctionLibrary"))
class THIRDWEB_API UThirdwebFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	/** Converts a private key into a wallet handle */
	UFUNCTION(BlueprintPure, meta=(DisplayName="To Wallet", CompactNodeTitle="->", BlueprintAutocast), Category="Utilities|Wallet")
	static FWalletHandle Conv_StringToWalletHandle(FString PrivateKey);

	/** Returns true if A is equal to B (A == B) */
	UFUNCTION(BlueprintPure, meta=(DisplayName="Equal (Wallet)", CompactNodeTitle="==", Keywords="== equal"), Category="Utilities|Operators")
	static bool EqualEqual_WalletHandleWalletHandle(FWalletHandle A, FWalletHandle B);

	/** Returns true if A is not equal to B (A != B) */
	UFUNCTION(BlueprintPure, meta=(DisplayName="Not Equal (Wallet)", CompactNodeTitle="!=", Keywords="!= not equal"), Category="Utilities|Operators")
	static bool NotEqual_WalletHandleWalletHandle(FWalletHandle A, FWalletHandle B);

	/** Generates a private key wallet handle */
	UFUNCTION(BlueprintPure, meta=(DisplayName="Generate Wallet"), Category="Thirdweb|Wallets")
	static FWalletHandle BP_GenerateWallet();

	/** Gets the public address of a wallet handle */
	UFUNCTION(BlueprintPure, meta=(DisplayName="To String (Wallet)", CompactNodeTitle="->", BlueprintAutocast), Category="Utilities|Wallet")
	static FString Conv_WalletHandleToString(FWalletHandle Wallet);

	/** Generates a private key wallet handle */
	UFUNCTION(BlueprintPure, meta=(DisplayName="Sign Message"), Category="Thirdweb|Wallets")
	static FString BP_LocalWalletSignMessage(const FWalletHandle& Wallet, const FString& Message);

	/** Check if the wallet handle is connected to a session */
	UFUNCTION(BlueprintPure, meta=(DisplayName="Is Connected"), Category="Thirdweb|Wallets")
	static bool BP_WalletIsConnected(const FWalletHandle& Wallet);

	/** Disconnect the wallet handle from a session. Blueprint Callable for side effects */
	UFUNCTION(BlueprintCallable, meta=(DisplayName="Disconnect"), Category="Thirdweb|Wallets")
	static void BP_DisconnectWallet(const FWalletHandle& Wallet);

	/** Disconnect the wallet handle from a session. Blueprint Callable for side effects */
	UFUNCTION(BlueprintCallable, meta=(DisplayName="Verify OTP", ExpandEnumAsExecs="ReturnValue"), Category="Thirdweb|Wallets|In App")
	static EOTPVerificationFunctionResult BP_VerifyOTP(FWalletHandle Wallet, const FString& OTP, FString& Error);
	
	/** Disconnect the wallet handle from a session. Blueprint Callable for side effects */
	UFUNCTION(BlueprintCallable, meta=(DisplayName="Send OTP", ExpandEnumAsExecs="ReturnValue"), Category="Thirdweb|Wallets|In App")
	static EFunctionResult BP_SendOTP(FWalletHandle Wallet, FString& Error);

	// Blueprint callable function to fetch OAuth login link
	UFUNCTION(BlueprintCallable, meta=(DisplayName="Fetch OAuth Login Link", ExpandEnumAsExecs="ReturnValue"), Category="Thirdweb|Wallets|In App")
	static EFunctionResult BP_FetchOAuthLoginLink(FWalletHandle Wallet, const FString& RedirectUrl, FString& LoginLink, FString& Error);
	
	/** Check if a wallet handle is valid */
	UFUNCTION(BlueprintPure, meta=(DisplayName="Is Valid"), Category="Thirdweb|Wallets")
	static bool BP_WalletIsValid(const FWalletHandle& Wallet);

	// Blueprint callable function to check if a smart wallet is deployed
	UFUNCTION(BlueprintCallable, Category="Thirdweb|Wallets|Smart Wallet", DisplayName="Is Deployed", meta=(ExpandEnumAsExecs="ReturnValue"))
	static ESmartWalletDeployedFunctionResult BP_IsSmartWalletDeployed(FWalletHandle Wallet, FString& Error);
	
	/** Convert a Thirdweb OAuth Provider to Text */
	UFUNCTION(BlueprintPure, meta=(DisplayName="To Text", CompactNodeTitle="->", BlueprintAutocast), Category="Utilities|Text")
	static FText Conv_ThirdwebOAuthProviderToText(EThirdwebOAuthProvider Provider);

	/** Convert a Thirdweb OAuth Provider to String */
	UFUNCTION(BlueprintPure, meta=(DisplayName="To String", CompactNodeTitle="->", BlueprintAutocast), Category="Utilities|String")
	static FString Conv_ThirdwebOAuthProviderToString(EThirdwebOAuthProvider Provider);
};
