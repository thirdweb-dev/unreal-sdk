// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

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

	namespace Maps
	{
		extern const TMap<EThirdwebOAuthProvider, FText> OAuthProviderToText;
	}
	/**
	 * Converts an EThirdwebOAuthProvider enum value to its corresponding FText representation.
	 *
	 * @param Provider The EThirdwebOAuthProvider enum value to convert.
	 * @return The FText representation of the specified EThirdwebOAuthProvider, or "Invalid" if the provider is not recognized.
	 */
	extern THIRDWEB_API FText ToText(const EThirdwebOAuthProvider Provider);

	/**
	 * Converts an EThirdwebOAuthProvider enum value to its string representation.
	 *
	 * @param Provider The EThirdwebOAuthProvider enum value to convert.
	 * @return The string representation of the specified EThirdwebOAuthProvider.
	 */
	extern THIRDWEB_API FString ToString(const EThirdwebOAuthProvider Provider);

	/**
	 * Converts the given FText to its corresponding EThirdwebOAuthProvider enum value.
	 *
	 * @param Text The FText representation of the OAuth provider to convert.
	 * @return The corresponding EThirdwebOAuthProvider enum value, or EThirdwebOAuthProvider::None if the provider is not recognized.
	 */
	extern THIRDWEB_API EThirdwebOAuthProvider ToOAuthProvider(const FText& Text);

	/**
	 * Converts the given FString to its corresponding EThirdwebOAuthProvider enum value.
	 *
	 * @param String The FString representation of the OAuth provider to convert.
	 * @return The corresponding EThirdwebOAuthProvider enum value, or EThirdwebOAuthProvider::None if the provider is not recognized.
	 */
	extern THIRDWEB_API EThirdwebOAuthProvider ToOAuthProvider(const FString& String);

	namespace Json
	{
		extern TSharedPtr<FJsonObject> ToJson(const FString& String);
		extern TArray<TSharedPtr<FJsonValue>> ToJsonArray(const FString& String);
		extern FString ToString(const TSharedPtr<FJsonObject>& JsonObject);
	}
}

