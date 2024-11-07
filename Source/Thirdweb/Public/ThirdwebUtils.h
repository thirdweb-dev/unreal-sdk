// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "Serialization/JsonTypes.h"
#include "Dom/JsonObject.h"

enum class EThirdwebEngineTransactionOnChainStatus : uint8;
enum class EThirdwebEngineTransactionStatus : uint8;
struct FWalletHandle;
struct FInAppWalletHandle;
struct FSmartWalletHandle;
class FJsonValue;
class IHttpRequest;
enum class EThirdwebOAuthProvider : uint8;

namespace ThirdwebUtils
{
	extern const TCHAR* ZeroAddress;

	/**
	 * Checks if the provided Ethereum address is checksummed.
	 *
	 * @param Address The Ethereum address to check.
	 * @return True if the address is checksummed, false otherwise.
	 */
	extern THIRDWEB_API bool IsChecksummedAddress(const FString& Address);

	/**
	 * Checks if the provided Ethereum address is valid.
	 *
	 * @param Address The Ethereum address to check.
	 * @param bWithChecksum Set to true to validate the address with checksum.
	 * @return True if the address is valid, false otherwise.
	 */
	extern THIRDWEB_API bool IsValidAddress(const FString& Address, const bool bWithChecksum = false);

	/**
	 * Converts the provided Ethereum address to a checksummed address.
	 *
	 * @param Address The Ethereum address to convert.
	 * @return The checksummed Ethereum address.
	 */
	extern THIRDWEB_API FString ToChecksummedAddress(const FString& Address);

	/**
	 * Converts an EThirdwebOAuthProvider enum value to its corresponding FText representation.
	 *
	 * @param Provider The EThirdwebOAuthProvider enum value to convert.
	 * @return The FText representation of the specified EThirdwebOAuthProvider, or "Invalid" if the provider is not recognized.
	 */
	extern THIRDWEB_API FText ToText(const EThirdwebOAuthProvider Provider);
	extern THIRDWEB_API FText ToText(const EThirdwebEngineTransactionStatus Status);
	extern THIRDWEB_API FText ToText(const EThirdwebEngineTransactionOnChainStatus Status);

	/**
	 * Converts an EThirdwebOAuthProvider enum value to its string representation.
	 *
	 * @param Provider The EThirdwebOAuthProvider enum value to convert.
	 * @return The string representation of the specified EThirdwebOAuthProvider.
	 */
	extern THIRDWEB_API FString ToString(const EThirdwebOAuthProvider Provider);
	extern THIRDWEB_API FString ToString(const EThirdwebEngineTransactionStatus Status);
	extern THIRDWEB_API FString ToString(const EThirdwebEngineTransactionOnChainStatus Status);

	/**
	 * Converts the given FText to its corresponding EThirdwebOAuthProvider enum value.
	 *
	 * @param Text The FText representation of the OAuth provider to convert.
	 * @return The corresponding EThirdwebOAuthProvider enum value, or EThirdwebOAuthProvider::None if the provider is not recognized.
	 */
	extern THIRDWEB_API EThirdwebOAuthProvider ToOAuthProvider(const FText& Text);
	extern THIRDWEB_API EThirdwebEngineTransactionStatus ToTransactionStatus(const FText& Text);
	extern THIRDWEB_API EThirdwebEngineTransactionOnChainStatus ToOnChainStatus(const FText& Text);

	/**
	 * Converts the given FString to its corresponding EThirdwebOAuthProvider enum value.
	 *
	 * @param String The FString representation of the OAuth provider to convert.
	 * @return The corresponding EThirdwebOAuthProvider enum value, or EThirdwebOAuthProvider::None if the provider is not recognized.
	 */
	extern THIRDWEB_API EThirdwebOAuthProvider ToOAuthProvider(const FString& String);
	extern THIRDWEB_API EThirdwebEngineTransactionStatus ToTransactionStatus(const FString& String);
	extern THIRDWEB_API EThirdwebEngineTransactionOnChainStatus ToOnChainStatus(const FString& String);


	extern FString ParseAuthResult(const FString& AuthResult);
	
	namespace Maps
	{
		extern const TMap<EThirdwebOAuthProvider, FText> OAuthProviderToText;
		extern const TMap<EThirdwebEngineTransactionStatus, FText> TransactionStatusToText;
		extern const TMap<EThirdwebEngineTransactionOnChainStatus, FText> OnChainStatusToText;
	}
	
	namespace Json
	{
		extern TSharedPtr<FJsonObject> ToJson(const FString& String);
		extern TArray<TSharedPtr<FJsonValue>> ToJsonArray(const FString& String);
		extern FString ToString(const TSharedPtr<FJsonObject>& JsonObject);
		extern FString AsString(const TSharedPtr<FJsonValue>& JsonValue);
		extern bool ParseEngineResponse(const FString& Content, TSharedPtr<FJsonObject>& JsonObject, FString& Error);
		
		template <EJson JsonType>
		bool IsNotNullField(const TSharedPtr<FJsonObject>& JsonObject, const FStringView FieldName)
		{
			return JsonObject.IsValid() && JsonObject->HasField(FieldName) ? !JsonObject->GetField(FieldName, JsonType)->IsNull() : false;
		}
	}

	namespace Internal
	{
		extern FString MaskSensitiveString(const FString& InString, const FString& MatchString, const FString& MaskCharacter = TEXT("*"), const int32 ShowBeginCount = 4, const int32 ShowEndCount = 4);
		extern TArray<FString> MaskSensitiveString(const TArray<FString>& InStrings,
		                                           const FString& MatchString,
		                                           const FString& MaskCharacter = TEXT("*"),
		                                           const int32 ShowBeginCount = 4,
		                                           const int32 ShowEndCount = 4);
		extern TArray<FString> MaskSensitiveString(const TArray<FString>& InStrings,
		                                           const TArray<FString>& MatchStrings,
		                                           const FString& MaskCharacter = TEXT("*"),
		                                           const int32 ShowBeginCount = 4,
		                                           const int32 ShowEndCount = 4);
		extern void LogRequest(const TSharedRef<IHttpRequest>& Request, const TArray<FString>& SensitiveStrings = {});

		extern int64 ParseInt64(const FString& String);

		extern FString GetPluginVersion();
		extern FString GenerateUUID();

		static void SendConnectEvent(const FWalletHandle Wallet);
		static void SendConnectEvent(const FInAppWalletHandle Wallet);
		static void SendConnectEvent(const FSmartWalletHandle Wallet);

		extern TSharedRef<IHttpRequest> CreateEngineRequest(const FString& Verb = TEXT("GET"));
	}
}
