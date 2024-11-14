// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "ThirdwebFunctionLibrary.generated.h"

struct FThirdwebEngineTransactionStatusResult;
struct FThirdwebCountryCode;
struct FInAppWalletHandle;
struct FSmartWalletHandle;
enum class EFunctionResult : uint8;
enum class EThirdwebOAuthProvider : uint8;

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

	/** Check if the wallet handle is connected to a session */
	UFUNCTION(BlueprintPure, meta=(DisplayName="Is Connected"), Category="Thirdweb|Wallets|In App")
	static bool BP_WalletIsConnected(const FInAppWalletHandle& Wallet);

	/** Disconnect the wallet handle from a session. Blueprint Callable for side effects */
	UFUNCTION(BlueprintCallable, meta=(DisplayName="Disconnect"), Category="Thirdweb|Wallets|In App")
	static void BP_DisconnectWallet(const FInAppWalletHandle& Wallet);

	// Fetch OAuth login link
	UFUNCTION(BlueprintCallable, meta=(DisplayName="Fetch OAuth Login Link", ExpandEnumAsExecs="ReturnValue"), Category="Thirdweb|Wallets|In App")
	static EFunctionResult BP_FetchOAuthLoginLink(FInAppWalletHandle Wallet, const FString& RedirectUrl, FString& LoginLink, FString& Error);
	
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
	UFUNCTION(BlueprintPure, DisplayName="To Checksummed Address", meta=(AutoCreateRefTerm="Address"), Category="Utilities|Text")
	static FText Conv_TextAddressToStringChecksummedAddress(const FText& Address);

	/** Constant for the ethereum zero address */
	UFUNCTION(BlueprintPure, DisplayName="Zero Address", Category="Utilities|String")
	static FString BP_ZeroAddress();

	/// Data

	/** Gets the country code data for the input country code, if it exists */
	UFUNCTION(BlueprintPure, DisplayName="Get Country Code Data", Category="Utilities|Country Codes")
	static FThirdwebCountryCode BP_GetCountryCodeData(const int32 CountryCode);

	/** Gets all country code data */
	UFUNCTION(BlueprintPure, DisplayName="Get All Country Code Data", Category="Utilities|Country Codes")
	static TArray<FThirdwebCountryCode> BP_GetAllCountryCodeData();

	// Engine

	UFUNCTION(BlueprintPure, DisplayName="Get Transaction Receipt Inputs", Category="Utilities|Engine|Transaction Status Result")
	static void BP_GetTransactionReceiptInputs(const FThirdwebEngineTransactionStatusResult& TransactionStatus, FString& TxHash, int64& ChainID);
};
