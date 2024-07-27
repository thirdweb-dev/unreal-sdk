// Copyright (c) 2024 Thirdweb. All Rights Reserved.

// ReSharper disable CppUE4CodingStandardNamingViolationWarning,CppUE4CodingStandardNamingViolationWarning
#pragma once

#include <cstdarg>
#include <cstdint>
#include <cstdlib>
#include <ostream>
#include <new>

enum class EFunctionResult : uint8;

namespace Thirdweb
{
	struct FFIResult
	{
		friend class UThirdwebSubsystem;

		bool success;
		const char* message;

	private:
		const TCHAR* Message() const { return StringCast<TCHAR>(message).Get(); }
		// Helper function to convert to separate operation results
		bool AssignResult(bool& bCanRetry, FString& Output, bool bErrorOnlyResult = false);
		EFunctionResult AssignBPResult(FString& Output);
	};

	extern "C" {
	void free_ffi_result(FFIResult result);

	FFIResult create_private_key_wallet(const char* private_key);

	FFIResult generate_private_key_wallet();

	FFIResult create_in_app_wallet(const char* client_id,
	                               const char* bundle_id,
	                               const char* secret_key,
	                               const char* email,
	                               const char* storage_directory_path,
	                               const char* oauth_provider);

	FFIResult in_app_wallet_send_otp(uintptr_t handle_id);

	FFIResult in_app_wallet_verify_otp(uintptr_t handle_id, const char* otp);

	FFIResult in_app_wallet_fetch_oauth_login_link(uintptr_t handle_id, const char* redirect_url);

	FFIResult in_app_wallet_sign_in_with_oauth(uintptr_t handle_id, const char* auth_result);

	FFIResult create_smart_wallet(const char* client_id,
	                              const char* bundle_id,
	                              const char* secret_key,
	                              uintptr_t personal_wallet_handle_id,
	                              const char* chain_id,
	                              bool gasless,
	                              const char* factory,
	                              const char* account_override);

	FFIResult smart_wallet_is_deployed(uintptr_t handle_id);

	FFIResult smart_wallet_get_all_admins(uintptr_t handle_id);

	FFIResult smart_wallet_get_all_active_signers(uintptr_t handle_id);

	FFIResult smart_wallet_create_session_key(uintptr_t handle_id,
	                                          const char* signer_address,
	                                          const char* is_admin,
	                                          const char* const * approved_targets,
	                                          uintptr_t approved_targets_count,
	                                          const char* native_token_limit_per_transaction_in_wei,
	                                          const char* permission_start_timestamp,
	                                          const char* permission_end_timestamp,
	                                          const char* req_validity_start_timestamp,
	                                          const char* req_validity_end_timestamp);

	FFIResult get_wallet_address(uintptr_t handle_id);

	FFIResult sign_message(uintptr_t handle_id, const char* message);

	FFIResult is_connected(uintptr_t handle_id);

	FFIResult disconnect(uintptr_t handle_id);

	void free_wallet(uintptr_t handle_id);

	void free_string(char* s);
	} // extern "C"
} // namespace Thirdweb
