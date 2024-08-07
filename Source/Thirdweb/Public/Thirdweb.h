// Copyright (c) 2024 Thirdweb. All Rights Reserved.

// ReSharper disable CppUE4CodingStandardNamingViolationWarning,CppUE4CodingStandardNamingViolationWarning
#pragma once

#include <cstdarg>
#include <cstdint>
#include <cstdlib>
#include <ostream>
#include <new>

#include "Hash/Keccak256.h"

enum class EFunctionResult : uint8;

namespace Thirdweb
{
	struct FFIResult
	{
		friend class UThirdwebSubsystem;
		friend struct FWalletHandle;
		bool success;
		const char *message;

	private:
		const TCHAR *Message() const { return StringCast<TCHAR, 1024, ANSICHAR>(message).Get(); }
		// Assign's result to variables and then frees the underlying FFIResult
		bool AssignResult(FString &Output, bool bErrorOnlyResult = false) const;
		// Assign's result to variables including retry and then frees the underlying FFIResult
		bool AssignRetryResult(bool &bCanRetry, FString &Output, bool bErrorOnlyResult = false) const;
		// Assign's result to output directly
		FString GetOutput() const;
		// Frees the FFI Result for functions that have no relevant output
		void Free() const;
		// Convenience function to log the FFIResult
		void Log() const;
	};

	static const char *GetOrNull(const FString &In) { return In.TrimStartAndEnd().IsEmpty() ? nullptr : StringCast<ANSICHAR>(*In.TrimStartAndEnd()).Get(); }
	static bool IsHex(const FString &In) { return FRegexMatcher(FRegexPattern(TEXT("^((-)?0x[0-9a-f]+|(0x))$"), ERegexPatternFlags::CaseInsensitive), In).FindNext(); }
	static bool IsAddressLike(const FString &In) { return FRegexMatcher(FRegexPattern(TEXT("^(0x)?[0-9a-f]{40}$"), ERegexPatternFlags::CaseInsensitive), In).FindNext(); }

	static bool IsChecksummedAddress(const FString &In)
	{
		if (!IsAddressLike(In))
		{
			return false;
		}
		FString Address = In;

		// Convert the correct hash into a hex string
		FString addressHash = FKeccak256().KeccakFromString(In, true);

		for (int32 i = 0; i < 40; ++i)
		{
			// ReSharper disable once CppTooWideScopeInitStatement
			int32 HexValue = FParse::HexDigit(addressHash[i]);
			if ((HexValue > 7 && FChar::ToUpper(Address[i]) != Address[i]) || (HexValue <= 7 && FChar::ToLower(Address[i]) != Address[i]))
			{
				return false;
			}
		}
		return true;
	}

	static bool IsValidAddress(const FString &In, const bool bWithChecksum = false)
	{
		FString Value = In;
		if (!IsHex(Value))
		{
			Value = Value.ToLower().StartsWith(TEXT("0x")) ? Value : TEXT("0x") + Value;
		}
		// basic check
		if (!IsAddressLike(Value))
		{
			return false;
		}
		// all lower or all upper
		if (FRegexMatcher(FRegexPattern(TEXT("^(0x|0X)?[0-9a-f]{40}$")), Value).FindNext(), FRegexMatcher(FRegexPattern(TEXT("^(0x|0X)?[0-9A-F]{40}$")), Value).FindNext())
		{
			return true;
		}

		return bWithChecksum ? IsChecksummedAddress(Value) : true;
	}

	extern "C"
	{

		void free_ffi_result(FFIResult result);

		FFIResult create_private_key_wallet(const char *private_key);

		FFIResult generate_private_key_wallet();

		FFIResult create_in_app_wallet(const char *client_id,
									   const char *bundle_id,
									   const char *secret_key,
									   const char *email,
									   const char *storage_directory_path,
									   const char *oauth_provider);

		FFIResult in_app_wallet_send_otp(uintptr_t handle_id);

		FFIResult in_app_wallet_verify_otp(uintptr_t handle_id, const char *otp);

		FFIResult in_app_wallet_fetch_oauth_login_link(uintptr_t handle_id, const char *redirect_url);

		FFIResult in_app_wallet_sign_in_with_oauth(uintptr_t handle_id, const char *auth_result);

		FFIResult create_smart_wallet(const char *client_id,
									  const char *bundle_id,
									  const char *secret_key,
									  uintptr_t personal_wallet_handle_id,
									  const char *chain_id,
									  bool gasless,
									  const char *factory,
									  const char *account_override);

		FFIResult smart_wallet_is_deployed(uintptr_t handle_id);

		FFIResult smart_wallet_get_all_admins(uintptr_t handle_id);

		FFIResult smart_wallet_get_all_active_signers(uintptr_t handle_id);

		FFIResult smart_wallet_create_session_key(uintptr_t handle_id,
												  const char *signer_address,
												  const char *const *approved_targets,
												  uintptr_t approved_targets_count,
												  const char *native_token_limit_per_transaction_in_wei,
												  uint64_t permission_start_timestamp,
												  uint64_t permission_end_timestamp,
												  uint64_t req_validity_start_timestamp,
												  uint64_t req_validity_end_timestamp);

		FFIResult smart_wallet_revoke_session_key(uintptr_t handle_id, const char *signer_address);

		FFIResult smart_wallet_add_admin(uintptr_t handle_id, const char *signer_address);

		FFIResult smart_wallet_remove_admin(uintptr_t handle_id, const char *signer_address);

		FFIResult get_wallet_address(uintptr_t handle_id);

		FFIResult sign_message(uintptr_t handle_id, const char *message);

		FFIResult is_connected(uintptr_t handle_id);

		FFIResult disconnect(uintptr_t handle_id);

		void free_wallet(uintptr_t handle_id);

		void free_string(char *s);

		FFIResult get_unix_timestamp_now();

		FFIResult get_unix_timestamp_in_ten_years();

		FFIResult is_valid_address(const char *address, bool check_checksum);

		FFIResult to_checksummed_address(const char *address);

		FFIResult is_valid_private_key(const char *private_key);

	} // extern "C"
} // namespace Thirdweb
