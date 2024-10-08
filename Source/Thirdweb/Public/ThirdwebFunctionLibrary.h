﻿// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

#include "ThirdwebFunctionLibrary.generated.h"


struct FInAppWalletHandle;
struct FSmartWalletHandle;
enum class EFunctionResult : uint8;
enum class EThirdwebOAuthProvider : uint8;
enum class EThirdwebOTPMethod : uint8;
enum class EOTPVerificationFunctionResult : uint8;
enum class ESmartWalletDeployedFunctionResult : uint8;

/**
 * Thirdweb Function Library
 */
UCLASS(meta=(BlueprintThreadSafe, DisplayName="Thirdweb", ScriptName="ThirdwebFunctionLibrary"))
class THIRDWEB_API UThirdwebFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/** Returns true if A is equal to B (A == B) */
	UFUNCTION(BlueprintPure, meta=(DisplayName="Equal (InApp Wallet)", CompactNodeTitle="==", Keywords="== equal"), Category="Utilities|Operators")
	static bool EqualEqual_InAppWalletHandleInAppWalletHandle(FInAppWalletHandle A, FInAppWalletHandle B);

	/** Returns true if A is not equal to B (A != B) */
	UFUNCTION(BlueprintPure, meta=(DisplayName="Not Equal (InApp Wallet)", CompactNodeTitle="!=", Keywords="!= not equal"), Category="Utilities|Operators")
	static bool NotEqual_InAppWalletHandleInAppWalletHandle(FInAppWalletHandle A, FInAppWalletHandle B);
	
	/** Returns true if A is equal to B (A == B) */
	UFUNCTION(BlueprintPure, meta=(DisplayName="Equal (Smart Wallet)", CompactNodeTitle="==", Keywords="== equal"), Category="Utilities|Operators")
	static bool EqualEqual_SmartWalletHandleSmartWalletHandle(FSmartWalletHandle A, FSmartWalletHandle B);

	/** Returns true if A is not equal to B (A != B) */
	UFUNCTION(BlueprintPure, meta=(DisplayName="Not Equal (Smart Wallet)", CompactNodeTitle="!=", Keywords="!= not equal"), Category="Utilities|Operators")
	static bool NotEqual_SmartWalletHandleSmartWalletHandle(FSmartWalletHandle A, FSmartWalletHandle B);

	/** Gets the public address of an InApp wallet handle */
	UFUNCTION(BlueprintPure, meta=(DisplayName="Get Address", BlueprintAutocast), Category="Utilities|String|In App Wallet")
	static FString Conv_InAppWalletHandleToString(FInAppWalletHandle Wallet);

	/** Gets the public address of a smart wallet handle */
	UFUNCTION(BlueprintPure, meta=(DisplayName="Get Address", BlueprintAutocast), Category="Utilities|String|Smart Wallet")
	static FString Conv_SmartWalletHandleToString(FSmartWalletHandle Wallet);

	/** Gets the public address of an InApp wallet handle */
	UFUNCTION(BlueprintPure, meta=(DisplayName="Get Address", BlueprintAutocast), Category="Utilities|Text|In App Wallet")
	static FText Conv_InAppWalletHandleToText(FInAppWalletHandle Wallet);

	/** Gets the public address of a smart wallet handle */
	UFUNCTION(BlueprintPure, meta=(DisplayName="Get Address", BlueprintAutocast), Category="Utilities|Text|Smart Wallet")
	static FText Conv_SmartWalletHandleToText(FSmartWalletHandle Wallet);

	UFUNCTION(BlueprintCallable, DisplayName="Create Email Wallet", meta=(ExpandEnumAsExecs="ReturnValue", AdvancedDisplay="PartnerId"), Category="Thirdweb|Wallets|In App")
	static EFunctionResult BP_CreateInAppEmailWallet(const FString& Email, const FString& PartnerId, FInAppWalletHandle& Wallet, FString& Error);

	UFUNCTION(BlueprintCallable, DisplayName="Create OAuth Wallet", meta=(ExpandEnumAsExecs="ReturnValue", AdvancedDisplay="PartnerId"), Category="Thirdweb|Wallets|In App")
	static EFunctionResult BP_CreateInAppOAuthWallet(const EThirdwebOAuthProvider Provider, const FString& PartnerId, FInAppWalletHandle& Wallet, FString& Error);

	UFUNCTION(BlueprintCallable, DisplayName="Create Phone Wallet", meta=(ExpandEnumAsExecs="ReturnValue", AdvancedDisplay="PartnerId"), Category="Thirdweb|Wallets|In App")
	static EFunctionResult BP_CreateInAppPhoneWallet(const FString& Phone, const FString& PartnerId, FInAppWalletHandle& Wallet, FString& Error);

	UFUNCTION(BlueprintCallable, DisplayName="Create JWT Wallet", meta=(ExpandEnumAsExecs="ReturnValue", AdvancedDisplay="PartnerId"), Category="Thirdweb|Wallets|In App")
	static EFunctionResult BP_CreateInAppJwtWallet(const FString& PartnerId, FInAppWalletHandle& Wallet, FString& Error);

	UFUNCTION(BlueprintCallable, DisplayName="Create Auth Endpoint Wallet", meta=(ExpandEnumAsExecs="ReturnValue", AdvancedDisplay="PartnerId"), Category="Thirdweb|Wallets|In App")
	static EFunctionResult BP_CreateInAppAuthEndpointWallet(const FString& PartnerId, FInAppWalletHandle& Wallet, FString& Error);

	UFUNCTION(BlueprintCallable, DisplayName="Create Guest Wallet", meta=(ExpandEnumAsExecs="ReturnValue", AdvancedDisplay="PartnerId"), Category="Thirdweb|Wallets|In App")
	static EFunctionResult BP_CreateInAppGuestWallet(const FString& PartnerId, FInAppWalletHandle& Wallet, FString& Error);

	UFUNCTION(BlueprintCallable, DisplayName="Create Smart Wallet",
		meta=(ExpandEnumAsExecs="ReturnValue", AdvancedDisplay="bGasless,Factory,AccountOverride", AutoCreateRefTerm="Factory,AccountOverride"), Category="Thirdweb|Wallets|Smart")
	static EFunctionResult BP_CreateSmartWallet(const FInAppWalletHandle PersonalWallet,
	                                            FSmartWalletHandle& SmartWallet,
	                                            FString& Error,
	                                            const int64 ChainID,
	                                            const bool bGasless = true,
	                                            const FString& Factory = "",
	                                            const FString& AccountOverride = "");

	/** Signs an arbitrary message with an InApp wallet */
	UFUNCTION(BlueprintPure, meta=(DisplayName="Sign Message"), Category="Thirdweb|Wallets")
	static FString BP_SignInAppMessage(const FInAppWalletHandle& Wallet, const FString& Message);
	
	/** Signs an arbitrary message with a smart wallet */
	UFUNCTION(BlueprintPure, meta=(DisplayName="Sign Message"), Category="Thirdweb|Wallets")
	static FString BP_SignSmartMessage(const FSmartWalletHandle& Wallet, const FString& Message);

	/** Check if the wallet handle is connected to a session */
	UFUNCTION(BlueprintPure, meta=(DisplayName="Is Connected"), Category="Thirdweb|Wallets|In App")
	static bool BP_WalletIsConnected(const FInAppWalletHandle& Wallet);

	/** Disconnect the wallet handle from a session. Blueprint Callable for side effects */
	UFUNCTION(BlueprintCallable, meta=(DisplayName="Disconnect"), Category="Thirdweb|Wallets|In App")
	static void BP_DisconnectWallet(const FInAppWalletHandle& Wallet);

	// Fetch OAuth login link
	UFUNCTION(BlueprintCallable, meta=(DisplayName="Fetch OAuth Login Link", ExpandEnumAsExecs="ReturnValue"), Category="Thirdweb|Wallets|In App")
	static EFunctionResult BP_FetchOAuthLoginLink(FInAppWalletHandle Wallet, const FString& RedirectUrl, FString& LoginLink, FString& Error);

	// Sign in with the oauth payload received from your login flow
	UFUNCTION(BlueprintCallable, meta=(DisplayName="Sign In With OAuth", ExpandEnumAsExecs="ReturnValue"), Category="Thirdweb|Wallets|In App")
	static EFunctionResult BP_SignInWithOAuth(FInAppWalletHandle Wallet, const FString& AuthResult, FString& Error);

	// Sign in with the oauth payload received from your login flow
	UFUNCTION(BlueprintCallable, meta=(DisplayName="Sign In With JWT", ExpandEnumAsExecs="ReturnValue"), Category="Thirdweb|Wallets|In App")
	static EFunctionResult BP_SignInWithJwt(FInAppWalletHandle Wallet, const FString& Jwt, FString& Error);

	// Sign in with the oauth payload received from your login flow
	UFUNCTION(BlueprintCallable, meta=(DisplayName="Sign In With Auth Endpoint", ExpandEnumAsExecs="ReturnValue"), Category="Thirdweb|Wallets|In App")
	static EFunctionResult BP_SignInWithAuthEndpoint(FInAppWalletHandle Wallet, const FString& Payload, FString& Error);

	// Sign in with the oauth payload received from your login flow
	UFUNCTION(BlueprintCallable, meta=(DisplayName="Sign In With Guest", ExpandEnumAsExecs="ReturnValue"), Category="Thirdweb|Wallets|In App")
	static EFunctionResult BP_SignInWithGuest(FInAppWalletHandle Wallet, FString& Error);
	
	/** Check if an InApp wallet handle is valid */
	UFUNCTION(BlueprintPure, meta=(DisplayName="Is Valid"), Category="Thirdweb|Wallets|In App")
	static bool BP_InAppWalletIsValid(const FInAppWalletHandle& Wallet);
	
	/** Check if a smart wallet handle is valid */
	UFUNCTION(BlueprintPure, meta=(DisplayName="Is Valid"), Category="Thirdweb|Wallets|Smart")
	static bool BP_SmartWalletIsValid(const FSmartWalletHandle& Wallet);

	/** Convert a Thirdweb OAuth Provider to Text */
	UFUNCTION(BlueprintPure, meta=(DisplayName="To Text (Thirdweb OAuth Provider)", CompactNodeTitle="->", BlueprintAutocast), Category="Utilities|Text")
	static FText Conv_ThirdwebOAuthProviderToText(EThirdwebOAuthProvider Provider);

	/** Convert a Thirdweb OAuth Provider to String */
	UFUNCTION(BlueprintPure, meta=(DisplayName="To String (Thirdweb OAuth Provider)", CompactNodeTitle="->", BlueprintAutocast), Category="Utilities|String")
	static FString Conv_ThirdwebOAuthProviderToString(EThirdwebOAuthProvider Provider);

	/** Convert a Thirdweb OAuth Provider to Text */
	UFUNCTION(BlueprintPure, meta=(DisplayName="To Thirdweb OAuth Provider", CompactNodeTitle="->", BlueprintAutocast), Category="Utilities|Text")
	static EThirdwebOAuthProvider Conv_TextToThirdwebOAuthProvider(FText Text);

	/** Convert a Thirdweb OAuth Provider to String */
	UFUNCTION(BlueprintPure, meta=(DisplayName="To Thirdweb OAuth Provider", CompactNodeTitle="->", BlueprintAutocast), Category="Utilities|String")
	static EThirdwebOAuthProvider Conv_StringToThirdwebOAuthProvider(FString String);

	/** Checks the validity of the address */
	UFUNCTION(BlueprintPure, meta=(DisplayName="Is Valid Address"), Category="Utilities|String")
	static bool BP_IsStringValidAddress(const FString& Address, const bool bWithChecksum = false);

	/** Checks the checksum of the address */
	UFUNCTION(BlueprintPure, meta=(DisplayName="Is Checksummed Address"), Category="Utilities|String")
	static bool BP_IsStringChecksummedAddress(const FString& Address);

	/** Returns the checksummed address. If already checksummed it is a no-op */
	UFUNCTION(BlueprintPure, meta=(DisplayName="To Checksummed Address"), Category="Utilities|String")
	static FString Conv_StringAddressToStringChecksummedAddress(const FString& Address);

	/** Checks the validity of the address */
	UFUNCTION(BlueprintPure, meta=(DisplayName="Is Valid Address"), Category="Utilities|Text")
	static bool BP_IsTextValidAddress(const FText Address, const bool bWithChecksum = false);

	/** Checks the checksum of the address */
	UFUNCTION(BlueprintPure, meta=(DisplayName="Is Checksummed Address"), Category="Utilities|Text")
	static bool BP_IsTextChecksummedAddress(const FText Address);
	
	/** Returns the checksummed address. If already checksummed it is a no-op */
	UFUNCTION(BlueprintPure, meta=(DisplayName="To Checksummed Address", AutoCreateRefTerm="Address"), Category="Utilities|Text")
	static FText Conv_TextAddressToStringChecksummedAddress(const FText& Address);

	UFUNCTION(BlueprintPure, meta=(DisplayName="Zero Address"), Category="Utilities|String")
	static FString BP_ZeroAddress();
};
