// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

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
	/** Converts a private key string into a wallet handle */
	UFUNCTION(BlueprintPure, meta=(DisplayName="Create PrivateKey Wallet", BlueprintAutocast), Category="Utilities|Wallet")
	static FWalletHandle Conv_StringToWalletHandle(FString PrivateKey);

	/** Converts a private key text into a wallet handle */
	UFUNCTION(BlueprintPure, meta=(DisplayName="Create PrivateKey Wallet", BlueprintAutocast), Category="Utilities|Wallet")
	static FWalletHandle Conv_TextToWalletHandle(FText PrivateKey) { return Conv_StringToWalletHandle(PrivateKey.ToString()); }

	/** Get the private key of a wallet handle - PrivateKey wallets only! */
	UFUNCTION(BlueprintPure, meta=(DisplayName="Get Private Key", BlueprintAutocast, ReturnDisplayName="Private Key"), Category="Thirdweb|Wallets")
	static FString BP_GetPrivateKeyString(const FWalletHandle& Wallet);

	/** Get the private key of a wallet handle - PrivateKey wallets only! */
	UFUNCTION(BlueprintPure, meta=(DisplayName="Get Private Key", BlueprintAutocast, ReturnDisplayName="Private Key"), Category="Thirdweb|Wallets")
	static FText BP_GetPrivateKeyText(const FWalletHandle& Wallet) { return FText::FromString(BP_GetPrivateKeyString(Wallet)); }
	
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

	/** Gets the public address of a wallet handle */
	UFUNCTION(BlueprintPure, meta=(DisplayName="To Text (Wallet)", CompactNodeTitle="->", BlueprintAutocast), Category="Utilities|Wallet")
	static FText Conv_WalletHandleToText(FWalletHandle Wallet) { return FText::FromString(Conv_WalletHandleToString(Wallet)); }
	
	UFUNCTION(BlueprintCallable, DisplayName="Create Email Wallet", meta=(ExpandEnumAsExecs="ReturnValue"), Category="Thirdweb|Wallets|In App")
	static EFunctionResult BP_CreateInAppEmailWallet(const FString& Email, FWalletHandle& Wallet, FString& Error);
	
	UFUNCTION(BlueprintCallable, DisplayName="Create OAuth Wallet", meta=(ExpandEnumAsExecs="ReturnValue"), Category="Thirdweb|Wallets|In App")
	static EFunctionResult BP_CreateInAppOAuthWallet(const EThirdwebOAuthProvider Provider, FWalletHandle& Wallet, FString& Error);

	UFUNCTION(BlueprintCallable, DisplayName="Create Smart Wallet", meta=(ExpandEnumAsExecs="ReturnValue", AdvancedDisplay="bGasless,Factory,AccountOverride", AutoCreateRefTerm="Factory,AccountOverride"), Category="Thirdweb|Wallets|Smart Wallet")
	static EFunctionResult BP_CreateSmartWallet(FWalletHandle PersonalWallet, FWalletHandle& SmartWallet, FString& Error, const int64 ChainID, const bool bGasless = true, const FString& Factory = "", const FString& AccountOverride = "");
	
	/** Signs an arbitrary message */
	UFUNCTION(BlueprintPure, meta=(DisplayName="Sign Message"), Category="Thirdweb|Wallets")
	static FString BP_SignMessage(const FWalletHandle& Wallet, const FString& Message);

	/** Check if the wallet handle is connected to a session */
	UFUNCTION(BlueprintPure, meta=(DisplayName="Is Connected"), Category="Thirdweb|Wallets")
	static bool BP_WalletIsConnected(const FWalletHandle& Wallet);

	/** Disconnect the wallet handle from a session. Blueprint Callable for side effects */
	UFUNCTION(BlueprintCallable, meta=(DisplayName="Disconnect"), Category="Thirdweb|Wallets")
	static void BP_DisconnectWallet(const FWalletHandle& Wallet);

	/** Verifies an OTP for the in-app wallet */
	UFUNCTION(BlueprintCallable, meta=(DisplayName="Verify OTP", ExpandEnumAsExecs="ReturnValue"), Category="Thirdweb|Wallets|In App")
	static EOTPVerificationFunctionResult BP_VerifyOTP(FWalletHandle Wallet, const FString& OTP, FString& Error);

	/** Sends an OTP for the in-app wallet */
	UFUNCTION(BlueprintCallable, meta=(DisplayName="Send OTP", ExpandEnumAsExecs="ReturnValue"), Category="Thirdweb|Wallets|In App")
	static EFunctionResult BP_SendOTP(FWalletHandle Wallet, FString& Error);

	// Fetch OAuth login link
	UFUNCTION(BlueprintCallable, meta=(DisplayName="Fetch OAuth Login Link", ExpandEnumAsExecs="ReturnValue"), Category="Thirdweb|Wallets|In App")
	static EFunctionResult BP_FetchOAuthLoginLink(FWalletHandle Wallet, const FString& RedirectUrl, FString& LoginLink, FString& Error);

	// Sign in with the oauth payload received from your login flow
	UFUNCTION(BlueprintCallable, meta=(DisplayName="Sign In With OAuth", ExpandEnumAsExecs="ReturnValue"), Category="Thirdweb|Wallets|In App")
	static EFunctionResult BP_SignInWithOAuth(FWalletHandle Wallet, const FString& AuthResult, FString& Error);
	
	/** Check if a wallet handle is valid */
	UFUNCTION(BlueprintPure, meta=(DisplayName="Is Valid"), Category="Thirdweb|Wallets")
	static bool BP_WalletIsValid(const FWalletHandle& Wallet);

	/** Check if a smart wallet is deployed */
	UFUNCTION(BlueprintCallable, Category="Thirdweb|Wallets|Smart Wallet", DisplayName="Is Deployed", meta=(ExpandEnumAsExecs="ReturnValue"))
	static ESmartWalletDeployedFunctionResult BP_IsSmartWalletDeployed(FWalletHandle Wallet, FString& Error);

	/** Create a session key for a smart wallet */
	UFUNCTION(BlueprintCallable, Category="Thirdweb|Wallets|Smart Wallet", DisplayName="Create Session Key",
		meta=(ExpandEnumAsExecs="ReturnValue", AdvancedDisplay="PermissionStart,PermissionEnd,RequestValidityStart,RequestValidityEnd", AutoCreateRefTerm=
			"PermissionStart,PermissionEnd,RequestValidityStart,RequestValidityEnd,ApprovedTargets,Signer,NativeTokenLimitPerTransactionInWei"))
	static EFunctionResult BP_CreateSmartWalletSessionKey(FWalletHandle Wallet, const FString& Signer, const TArray<FString>& ApprovedTargets,
	                                                      const FString& NativeTokenLimitPerTransactionInWei, const FDateTime& PermissionStart, const FDateTime& PermissionEnd,
	                                                      const FDateTime& RequestValidityStart, const FDateTime& RequestValidityEnd, FString& TransactionHash, FString& Error);

	/** Get all admins of a smart wallet */
	UFUNCTION(BlueprintCallable, Category="Thirdweb|Wallets|Smart Wallet", DisplayName="Get Admins", meta=(ExpandEnumAsExecs="ReturnValue"))
	static EFunctionResult BP_GetSmartWalletAdmins(FWalletHandle Wallet, TArray<FString>& Admins, FString& Error);

	/** Get all active signers of a smart wallet */
	UFUNCTION(BlueprintCallable, Category="Thirdweb|Wallets|Smart Wallet", DisplayName="Get Active Signers", meta=(ExpandEnumAsExecs="ReturnValue"))
	static EFunctionResult BP_GetSmartWalletActiveSigners(FWalletHandle Wallet, TArray<FSigner>& Signers, FString& Error);

	/** Revoke the session key of a smart wallet signer */
	UFUNCTION(BlueprintCallable, Category="Thirdweb|Wallets|Smart Wallet", DisplayName="Revoke Session Key", meta=(ExpandEnumAsExecs="ReturnValue"))
	static EFunctionResult BP_RevokeSmartWalletSessionKey(FWalletHandle Wallet, const FString& Signer, FString& Error);

	/** Add an admin signer to a smart wallet */
	UFUNCTION(BlueprintCallable, Category="Thirdweb|Wallets|Smart Wallet", DisplayName="Add Admin", meta=(ExpandEnumAsExecs="ReturnValue"))
	static EFunctionResult BP_AddSmartWalletAdmin(FWalletHandle Wallet, const FString& Signer, FString& Error);

	/** Remove an admin signer from a smart wallet */
	UFUNCTION(BlueprintCallable, Category="Thirdweb|Wallets|Smart Wallet", DisplayName="Remove Admin", meta=(ExpandEnumAsExecs="ReturnValue"))
	static EFunctionResult BP_RemoveSmartWalletAdmin(FWalletHandle Wallet, const FString& Signer, FString& Error);

	/** Convert a Thirdweb OAuth Provider to Text */
	UFUNCTION(BlueprintPure, meta=(DisplayName="To Text", CompactNodeTitle="->", BlueprintAutocast), Category="Utilities|Text")
	static FText Conv_ThirdwebOAuthProviderToText(EThirdwebOAuthProvider Provider);

	/** Convert a Thirdweb OAuth Provider to String */
	UFUNCTION(BlueprintPure, meta=(DisplayName="To String", CompactNodeTitle="->", BlueprintAutocast), Category="Utilities|String")
	static FString Conv_ThirdwebOAuthProviderToString(EThirdwebOAuthProvider Provider);

	/** Checks the validity of the address */
	UFUNCTION(BlueprintPure, meta=(DisplayName="Is Valid Address"), Category="Utilities|String")
	static bool BP_IsStringValidAddress(const FString& Address, const bool bWithChecksum = false);

	/** Checks the checksum of the address */
	UFUNCTION(BlueprintPure, meta=(DisplayName="Is Checksummed Address"), Category="Utilities|String")
	static bool BP_IsStringChecksummedAddress(const FString& Address);

	/** Checks the validity of the private key */
	UFUNCTION(BlueprintPure, meta=(DisplayName="Is Valid Private Key"), Category="Utilities|String")
	static bool BP_IsStringValidPrivateKey(const FString& PrivateKey);

	/** Returns the checksummed address. If already checksummed it is a no-op */
	UFUNCTION(BlueprintPure, meta=(DisplayName="To Checksummed Address"), Category="Utilities|String")
	static FString Conv_StringAddressToStringChecksummedAddress(const FString& Address);

	/** Checks the validity of the address */
	UFUNCTION(BlueprintPure, meta=(DisplayName="Is Valid Address", AutoCreateRefTerm="Address"), Category="Utilities|Text")
	static bool BP_IsTextValidAddress(const FText& Address, const bool bWithChecksum = false);

	/** Checks the checksum of the address */
	UFUNCTION(BlueprintPure, meta=(DisplayName="Is Checksummed Address", AutoCreateRefTerm="Address"), Category="Utilities|Text")
	static bool BP_IsTextChecksummedAddress(const FText& Address);

	/** Checks the validity of the private key */
	UFUNCTION(BlueprintPure, meta=(DisplayName="Is Valid Private Key", AutoCreateRefTerm="PrivateKey"), Category="Utilities|Text")
	static bool BP_IsTextValidPrivateKey(const FText& PrivateKey);

	/** Returns the checksummed address. If already checksummed it is a no-op */
	UFUNCTION(BlueprintPure, meta=(DisplayName="To Checksummed Address", AutoCreateRefTerm="Address"), Category="Utilities|Text")
	static FText Conv_TextAddressToStringChecksummedAddress(const FText& Address);

	UFUNCTION(BlueprintPure, meta=(DisplayName="Zero Address"), Category="Utilities|String")
	static FString BP_ZeroAddress() { return TEXT("0x0000000000000000000000000000000000000000"); }

	UFUNCTION(BlueprintPure, meta=(DisplayName="Is Active Signer"), Category="Utilities|String")
	static bool BP_IsActiveSigner(FWalletHandle Wallet, const FString& BackendWallet);
};
