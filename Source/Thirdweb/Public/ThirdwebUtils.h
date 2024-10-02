// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "Thirdweb.h"
#include "ThirdwebCommon.h"
#include "ThirdwebMacros.h"
#include "ThirdwebRuntimeSettings.h"

#define LOCTEXT_NAMESPACE "Thirdweb"

namespace ThirdwebUtils
{
	static const TCHAR* ZeroAddress = TEXT("0x0000000000000000000000000000000000000000");

	/**
	 * Checks if the provided Ethereum address is checksummed.
	 *
	 * @param Address The Ethereum address to check.
	 * @return True if the address is checksummed, false otherwise.
	 */
	static bool IsChecksummedAddress(const FString& Address) { return Thirdweb::is_valid_address(TO_RUST_STRING(Address), true).GetOutput().ToBool(); }

	/**
	 * Checks if the provided Ethereum address is valid.
	 *
	 * @param Address The Ethereum address to check.
	 * @param bWithChecksum Set to true to validate the address with checksum.
	 * @return True if the address is valid, false otherwise.
	 */
	static bool IsValidAddress(const FString& Address, const bool bWithChecksum = false) { return Thirdweb::is_valid_address(TO_RUST_STRING(Address), bWithChecksum).GetOutput().ToBool(); }
	
	/**
	 * Converts the provided Ethereum address to a checksummed address.
	 *
	 * @param Address The Ethereum address to convert.
	 * @return The checksummed Ethereum address.
	 */
	static FString ToChecksummedAddress(const FString& Address) { return Thirdweb::to_checksummed_address(TO_RUST_STRING(Address)).GetOutput(); }

	/**
	 * Converts an EThirdwebOAuthProvider enum value to its corresponding FText representation.
	 *
	 * @param Provider The EThirdwebOAuthProvider enum value to convert.
	 * @return The FText representation of the specified EThirdwebOAuthProvider, or "Invalid" if the provider is not recognized.
	 */
	static FText ToText(const EThirdwebOAuthProvider Provider)
	{
		static TMap<EThirdwebOAuthProvider, FText> Map = {
			{EThirdwebOAuthProvider::Google, LOCTEXT("Google", "Google")},
			{EThirdwebOAuthProvider::Apple, LOCTEXT("Apple", "Apple")},
			{EThirdwebOAuthProvider::Facebook, LOCTEXT("Facebook", "Facebook")},
			{EThirdwebOAuthProvider::Discord, LOCTEXT("Discord", "Discord")},
			{EThirdwebOAuthProvider::Farcaster, LOCTEXT("Farcaster", "Farcaster")},
			{EThirdwebOAuthProvider::Telegram, LOCTEXT("Telegram", "Telegram")},
			{EThirdwebOAuthProvider::Line, LOCTEXT("Line", "Line")},
			{EThirdwebOAuthProvider::X, LOCTEXT("X", "X")},
			{EThirdwebOAuthProvider::Coinbase, LOCTEXT("Coinbase", "Coinbase")}
		};
		return Map.Contains(Provider) ? Map[Provider] : FText::FromString(TEXT("Invalid"));
	}

	/**
	 * Converts an EThirdwebOAuthProvider enum value to its string representation.
	 *
	 * @param Provider The EThirdwebOAuthProvider enum value to convert.
	 * @return The string representation of the specified EThirdwebOAuthProvider.
	 */
	static FString ToString(const EThirdwebOAuthProvider Provider) { return ToText(Provider).ToString(); }

	/**
	 * Derives a client ID from the provided secret key.
	 *
	 * @param SecretKey The secret key used to compute the client ID.
	 * @return The computed client ID.
	 */
	static FString GetClientIdFromSecretKey(const FString& SecretKey) { return Thirdweb::compute_client_id_from_secret_key(TO_RUST_STRING(SecretKey)).GetOutput(); }
}

#undef LOCTEXT_NAMESPACE
