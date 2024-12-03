// Copyright (c) 2024 Thirdweb. All Rights Reserved.

// ReSharper disable CppUE4CodingStandardNamingViolationWarning,CppUE4CodingStandardNamingViolationWarning
#pragma once

#include <cstdarg>
#include <cstdint>
#include <cstdlib>
#include <new>
#include <ostream>

#include "ThirdwebMacros.h"

#include "Containers/StringConv.h"
#include "Containers/UnrealString.h"

#include "HAL/Platform.h"

namespace Thirdweb
{
	struct FFIResult
	{
		bool success;
		const char* message;

		// Assign's result to variables and then frees the underlying FFIResult
		bool AssignResult(FString& Output, const bool bErrorOnlyResult = false) const;
		// Frees the FFI Result for functions that have no relevant output
		void Free() const;
		// Convenience function to log the FFIResult
		void Log() const;
		// Assign's result to output directly
		FString GetOutput() const;
	};

	extern "C" {
		// Wallet Management

		/// Private Key Wallet

		FFIResult create_private_key_wallet(const char* private_key);
		FFIResult generate_private_key_wallet();
		FFIResult private_key_wallet_export(uintptr_t handle_id);

		/// In App Wallet

		FFIResult create_in_app_wallet(
			const char* client_id,
			const char* bundle_id,
			const char* secret_key,
			const char* email,
			const char* phone,
			const char* storage_directory_path,
			const char* auth_provider
		);
		FFIResult in_app_wallet_send_otp_email(uintptr_t handle_id);
		FFIResult in_app_wallet_sign_in_with_otp_email(uintptr_t handle_id, const char* otp);
		FFIResult in_app_wallet_send_otp_phone(uintptr_t handle_id);
		FFIResult in_app_wallet_sign_in_with_otp_phone(uintptr_t handle_id, const char* otp);
		FFIResult in_app_wallet_fetch_oauth_login_link(uintptr_t handle_id, const char* redirect_url);
		FFIResult in_app_wallet_sign_in_with_oauth(uintptr_t handle_id, const char* auth_result);
		FFIResult in_app_wallet_sign_in_with_jwt(uintptr_t handle_id, const char* jwt, const char* encryption_key);
		FFIResult in_app_wallet_sign_in_with_auth_endpoint(uintptr_t handle_id, const char* payload, const char* encryption_key);
		FFIResult in_app_wallet_sign_in_with_guest(uintptr_t handle_id, const char* session_id);
		FFIResult in_app_wallet_sign_in_with_siwe(intptr_t handle_id, const char* payload, const char* signature);

		// In App (Ecosystem) Wallets
		FFIResult create_ecosystem_wallet(
			const char* ecosystem_id,
			const char* ecosystem_partner_id,
			const char* client_id,
			const char* bundle_id,
			const char* secret_key,
			const char* email,
			const char* phone,
			const char* storage_directory_path,
			const char* auth_provider
		);
		FFIResult ecosystem_wallet_send_otp_email(uintptr_t handle_id);
		FFIResult ecosystem_wallet_sign_in_with_otp_email(uintptr_t handle_id, const char* otp);
		FFIResult ecosystem_wallet_send_otp_phone(uintptr_t handle_id);
		FFIResult ecosystem_wallet_sign_in_with_otp_phone(uintptr_t handle_id, const char* otp);
		FFIResult ecosystem_wallet_fetch_oauth_login_link(uintptr_t handle_id, const char* redirect_url);
		FFIResult ecosystem_wallet_sign_in_with_oauth(uintptr_t handle_id, const char* auth_result);
		FFIResult ecosystem_wallet_sign_in_with_jwt(uintptr_t handle_id, const char* jwt);
		FFIResult ecosystem_wallet_sign_in_with_auth_endpoint(uintptr_t handle_id, const char* payload);
		FFIResult ecosystem_wallet_sign_in_with_guest(uintptr_t handle_id, const char* session_id);
		FFIResult ecosystem_wallet_sign_in_with_siwe(intptr_t handle_id, const char* payload, const char* signature);

		FFIResult ecosystem_wallet_link_account(
			uintptr_t handle_id,
			uintptr_t wallet_to_link_handle_id,
			const char* otp,
			const char* oauth_result,
			const char* jwt,
			const char* payload,
			const char* session_id,
			const char* siwe_payload,
			const char* siwe_signature
		);
		FFIResult ecosystem_wallet_get_linked_accounts(uintptr_t handle_id);

		// Smart Wallet management

		FFIResult create_smart_wallet(
			const char* client_id,
			const char* bundle_id,
			const char* secret_key,
			uintptr_t personal_wallet_handle_id,
			const char* chain_id,
			bool gasless,
			const char* factory,
			const char* account_override
		);
		FFIResult smart_wallet_is_deployed(uintptr_t handle_id);
		FFIResult smart_wallet_get_all_admins(uintptr_t handle_id);
		FFIResult smart_wallet_get_all_active_signers(uintptr_t handle_id);
		FFIResult smart_wallet_create_session_key(
			uintptr_t handle_id,
			const char* signer_address,
			const char* const * approved_targets,
			uintptr_t approved_targets_count,
			const char* native_token_limit_per_transaction_in_wei,
			uint64_t permission_start_timestamp,
			uint64_t permission_end_timestamp,
			uint64_t req_validity_start_timestamp,
			uint64_t req_validity_end_timestamp
		);
		FFIResult smart_wallet_revoke_session_key(uintptr_t handle_id, const char* signer_address);
		FFIResult smart_wallet_add_admin(uintptr_t handle_id, const char* signer_address);
		FFIResult smart_wallet_remove_admin(uintptr_t handle_id, const char* signer_address);

		// Utility Methods

		FFIResult get_wallet_address(uintptr_t handle_id);
		FFIResult sign_message(uintptr_t handle_id, const char* message);
		FFIResult is_connected(uintptr_t handle_id);
		FFIResult disconnect(uintptr_t handle_id);
		void free_wallet(uintptr_t handle_id);
		void free_ffi_result(FFIResult result);
		void free_string(char* s);
		FFIResult is_valid_address(const char* address, bool check_checksum);
		FFIResult to_checksummed_address(const char* address);
		FFIResult is_valid_private_key(const char* private_key);
		FFIResult compute_client_id_from_secret_key(const char* secret_key);
		FFIResult get_unix_timestamp_now();
		FFIResult get_unix_timestamp_in_ten_years();
	}
}
