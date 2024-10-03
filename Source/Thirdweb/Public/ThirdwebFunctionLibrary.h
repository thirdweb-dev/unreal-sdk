// Copyright (c) 2024 Thirdweb. All Rights Reserved.

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

	/** Verifies an OTP for the in-app wallet */
	UFUNCTION(BlueprintCallable, meta=(DisplayName="Verify OTP", ExpandEnumAsExecs="ReturnValue"), Category="Thirdweb|Wallets|In App")
	static EFunctionResult BP_VerifyOTP(FInAppWalletHandle Wallet, const FString& OTP, FString& Error);

	/** Sends an OTP for the in-app wallet */
	UFUNCTION(BlueprintCallable, meta=(DisplayName="Send OTP", ExpandEnumAsExecs="ReturnValue"), Category="Thirdweb|Wallets|In App")
	static EFunctionResult BP_SendOTP(FInAppWalletHandle Wallet, FString& Error);

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

	/** Check if a smart wallet is deployed */
	UFUNCTION(BlueprintCallable, Category="Thirdweb|Wallets|Smart", DisplayName="Is Deployed", meta=(ExpandEnumAsExecs="ReturnValue"))
	static ESmartWalletDeployedFunctionResult BP_IsSmartWalletDeployed(FSmartWalletHandle Wallet, FString& Error);

	/** Create a session key for a smart wallet */
	UFUNCTION(BlueprintCallable, Category="Thirdweb|Wallets|Smart", DisplayName="Create Session Key",
		meta=(ExpandEnumAsExecs="ReturnValue", AdvancedDisplay="PermissionStart,PermissionEnd,RequestValidityStart,RequestValidityEnd", AutoCreateRefTerm=
			"PermissionStart,PermissionEnd,RequestValidityStart,RequestValidityEnd,ApprovedTargets,Signer,NativeTokenLimitPerTransactionInWei"))
	static EFunctionResult BP_CreateSmartWalletSessionKey(FSmartWalletHandle Wallet,
	                                                      const FString& Signer,
	                                                      const TArray<FString>& ApprovedTargets,
	                                                      const FString& NativeTokenLimitPerTransactionInWei,
	                                                      const FDateTime& PermissionStart,
	                                                      const FDateTime& PermissionEnd,
	                                                      const FDateTime& RequestValidityStart,
	                                                      const FDateTime& RequestValidityEnd,
	                                                      FString& TransactionHash,
	                                                      FString& Error);

	/** Get all admins of a smart wallet */
	UFUNCTION(BlueprintCallable, Category="Thirdweb|Wallets|Smart", DisplayName="Get Admins", meta=(ExpandEnumAsExecs="ReturnValue"))
	static EFunctionResult BP_GetSmartWalletAdmins(FSmartWalletHandle Wallet, TArray<FString>& Admins, FString& Error);

	/** Get all active signers of a smart wallet */
	UFUNCTION(BlueprintCallable, Category="Thirdweb|Wallets|Smart", DisplayName="Get Active Signers", meta=(ExpandEnumAsExecs="ReturnValue"))
	static EFunctionResult BP_GetSmartWalletActiveSigners(FSmartWalletHandle Wallet, TArray<FSigner>& Signers, FString& Error);

	/** Revoke the session key of a smart wallet signer */
	UFUNCTION(BlueprintCallable, Category="Thirdweb|Wallets|Smart", DisplayName="Revoke Session Key", meta=(ExpandEnumAsExecs="ReturnValue"))
	static EFunctionResult BP_RevokeSmartWalletSessionKey(FSmartWalletHandle Wallet, const FString& Signer, FString& Error);

	/** Add an admin signer to a smart wallet */
	UFUNCTION(BlueprintCallable, Category="Thirdweb|Wallets|Smart", DisplayName="Add Admin", meta=(ExpandEnumAsExecs="ReturnValue"))
	static EFunctionResult BP_AddSmartWalletAdmin(FSmartWalletHandle Wallet, const FString& Signer, FString& Error);

	/** Remove an admin signer from a smart wallet */
	UFUNCTION(BlueprintCallable, Category="Thirdweb|Wallets|Smart", DisplayName="Remove Admin", meta=(ExpandEnumAsExecs="ReturnValue"))
	static EFunctionResult BP_RemoveSmartWalletAdmin(FSmartWalletHandle Wallet, const FString& Signer, FString& Error);

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

	UFUNCTION(BlueprintPure, meta=(DisplayName="Is Active Signer"), Category="Thirdweb|Wallets|Smart")
	static bool BP_IsActiveSigner(FSmartWalletHandle Wallet, const FString& BackendWallet);
};
