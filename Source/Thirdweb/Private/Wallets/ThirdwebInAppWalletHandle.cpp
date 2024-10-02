// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "Wallets/ThirdwebInAppWalletHandle.h"

#include "Thirdweb.h"
#include "ThirdwebCommon.h"
#include "ThirdwebInternal.h"
#include "ThirdwebMacros.h"
#include "ThirdwebRuntimeSettings.h"
#include "ThirdwebUtils.h"

#include "GenericPlatform/GenericPlatformHttp.h"

#include "Kismet/KismetStringLibrary.h"

#include "Misc/DefaultValueHelper.h"

FInAppWalletHandle::FInAppWalletHandle()
{
	Type = InApp;
	Provider = EThirdwebOAuthProvider::None;
}

FInAppWalletHandle::FInAppWalletHandle(const EInAppSource InSource, const int64 InID)
{
	Provider = EThirdwebOAuthProvider::None;
	Type = InApp;
	ensureAlwaysMsgf(InSource != InvalidSource && InSource != OAuthProvider, TEXT("Invalid handle source"));
	Source = InSource;
	ensureAlwaysMsgf(InID > 0, TEXT("Invalid id 0"));
	ID = InID;
}

FInAppWalletHandle::FInAppWalletHandle(const EInAppSource InSource, const FString& Int64String)
{
	Provider = EThirdwebOAuthProvider::None;
	Type = InApp;
	ensureAlwaysMsgf(InSource != InvalidSource && InSource != OAuthProvider, TEXT("Invalid handle source"));
	Source = InSource;
	int64 InID;
	FDefaultValueHelper::ParseInt64(Int64String, InID);
	ensureAlwaysMsgf(InID > 0, TEXT("Invalid id 0"));
	ID = InID;
}

FInAppWalletHandle::FInAppWalletHandle(const EThirdwebOAuthProvider InProvider, const FString& Int64String)
{
	Type = InApp;
	Source = OAuthProvider;
	ensureAlwaysMsgf(InProvider != EThirdwebOAuthProvider::None, TEXT("Invalid oauth provider"));
	Provider = InProvider;
	int64 InID;
	FDefaultValueHelper::ParseInt64(Int64String, InID);
	ensureAlwaysMsgf(InID > 0, TEXT("Invalid id 0"));
	ID = InID;
}

bool FInAppWalletHandle::IsValid() const
{
	return Super::IsValid() && Source != InvalidSource;
}

bool FInAppWalletHandle::CreateEmailWallet(const FString& Email, FInAppWalletHandle& Wallet, FString& Error)
{
	if (Thirdweb::create_in_app_wallet(
		TO_RUST_STRING(UThirdwebRuntimeSettings::GetClientId()),
		TO_RUST_STRING(UThirdwebRuntimeSettings::GetBundleId()),
		nullptr,
		TO_RUST_STRING(Email),
		nullptr,
		TO_RUST_STRING(UThirdwebRuntimeSettings::GetStorageDirectory()),
		nullptr
	).AssignResult(Error))
	{
		Wallet = FInAppWalletHandle(EInAppSource::Email, Error);
		Error.Empty();
		return true;
	}
	return false;
}

bool FInAppWalletHandle::CreateEcosystemEmailWallet(const FString& PartnerId, const FString& Email, FInAppWalletHandle& Wallet, FString& Error)
{
	if (UThirdwebRuntimeSettings::GetEcosystemId().IsEmpty())
	{
		Error = TEXT("Ecosystem ID not set in settings");
		return false;
	}
	if (Thirdweb::create_ecosystem_wallet(
		TO_RUST_STRING(UThirdwebRuntimeSettings::GetEcosystemId()),
		TO_RUST_STRING(PartnerId),
		TO_RUST_STRING(UThirdwebRuntimeSettings::GetClientId()),
		TO_RUST_STRING(UThirdwebRuntimeSettings::GetBundleId()),
		nullptr,
		TO_RUST_STRING(Email),
		nullptr,
		TO_RUST_STRING(UThirdwebRuntimeSettings::GetStorageDirectory()),
		nullptr
	).AssignResult(Error))
	{
		Wallet = FInAppWalletHandle(EInAppSource::Email, Error);
		Error.Empty();
		return true;
	}
	return false;
}

bool FInAppWalletHandle::CreateOAuthWallet(const EThirdwebOAuthProvider Provider, FInAppWalletHandle& Wallet, FString& Error)
{
	if (Thirdweb::create_in_app_wallet(
		TO_RUST_STRING(UThirdwebRuntimeSettings::GetClientId()),
		TO_RUST_STRING(UThirdwebRuntimeSettings::GetBundleId()),
		nullptr,
		nullptr,
		nullptr,
		TO_RUST_STRING(UThirdwebRuntimeSettings::GetStorageDirectory()),
		TO_RUST_STRING(ThirdwebUtils::ToString(Provider))
	).AssignResult(Error))
	{
		Wallet = FInAppWalletHandle(Provider, Error);
		Error.Empty();
		return true;
	}
	return false;
}

bool FInAppWalletHandle::CreateEcosystemOAuthWallet(const FString& PartnerId, const EThirdwebOAuthProvider Provider, FInAppWalletHandle& Wallet, FString& Error)
{
	if (UThirdwebRuntimeSettings::GetEcosystemId().IsEmpty())
	{
		Error = TEXT("Ecosystem ID not set in settings");
		return false;
	}
	if (Thirdweb::create_ecosystem_wallet(
		TO_RUST_STRING(UThirdwebRuntimeSettings::GetEcosystemId()),
		TO_RUST_STRING(PartnerId),
		TO_RUST_STRING(UThirdwebRuntimeSettings::GetClientId()),
		TO_RUST_STRING(UThirdwebRuntimeSettings::GetBundleId()),
		nullptr,
		nullptr,
		nullptr,
		TO_RUST_STRING(UThirdwebRuntimeSettings::GetStorageDirectory()),
		TO_RUST_STRING(ThirdwebUtils::ToString(Provider))
	).AssignResult(Error))
	{
		Wallet = FInAppWalletHandle(Provider, Error);
		Error.Empty();
		return true;
	}
	return false;
}

bool FInAppWalletHandle::CreatePhoneWallet(const FString& Phone, FInAppWalletHandle& Wallet, FString& Error)
{
	if (Thirdweb::create_in_app_wallet(
		TO_RUST_STRING(UThirdwebRuntimeSettings::GetClientId()),
		TO_RUST_STRING(UThirdwebRuntimeSettings::GetBundleId()),
		nullptr,
		nullptr,
		TO_RUST_STRING(Phone),
		TO_RUST_STRING(UThirdwebRuntimeSettings::GetStorageDirectory()),
		nullptr
	).AssignResult(Error))
	{
		Wallet = FInAppWalletHandle(EInAppSource::Phone, Error);
		Error.Empty();
		return true;
	}
	return false;
}

bool FInAppWalletHandle::CreateEcosystemPhoneWallet(const FString& PartnerId, const FString& Phone, FInAppWalletHandle& Wallet, FString& Error)
{
	if (Thirdweb::create_ecosystem_wallet(
		TO_RUST_STRING(UThirdwebRuntimeSettings::GetEcosystemId()),
		TO_RUST_STRING(PartnerId),
		TO_RUST_STRING(UThirdwebRuntimeSettings::GetClientId()),
		TO_RUST_STRING(UThirdwebRuntimeSettings::GetBundleId()),
		nullptr,
		nullptr,
		TO_RUST_STRING(Phone),
		TO_RUST_STRING(UThirdwebRuntimeSettings::GetStorageDirectory()),
		nullptr
	).AssignResult(Error))
	{
		Wallet = FInAppWalletHandle(EInAppSource::Phone, Error);
		Error.Empty();
		return true;
	}
	return false;
}

bool FInAppWalletHandle::CreateCustomAuthWallet(const EInAppSource Source, FInAppWalletHandle& Wallet, FString& Error)
{
	if (Source != Jwt && Source != AuthEndpoint && Source != Guest)
	{
		Error = TEXT("Invalid custom auth source");
		return false;
	}

	if (Thirdweb::create_in_app_wallet(
		TO_RUST_STRING(UThirdwebRuntimeSettings::GetClientId()),
		TO_RUST_STRING(UThirdwebRuntimeSettings::GetBundleId()),
		nullptr,
		nullptr,
		nullptr,
		TO_RUST_STRING(UThirdwebRuntimeSettings::GetStorageDirectory()),
		TO_RUST_STRING(FString(GetSourceString(Source)))
	).AssignResult(Error))
	{
		Wallet = FInAppWalletHandle(Source, Error);
		Error.Empty();
		return true;
	}
	return false;
}

bool FInAppWalletHandle::CreateEcosystemCustomAuthWallet(const FString& PartnerId, const EInAppSource Source, FInAppWalletHandle& Wallet, FString& Error)
{
	if (Source != Jwt && Source != AuthEndpoint && Source != Guest)
	{
		Error = TEXT("Invalid custom auth source");
		return false;
	}
	if (Thirdweb::create_ecosystem_wallet(
		TO_RUST_STRING(UThirdwebRuntimeSettings::GetEcosystemId()),
		TO_RUST_STRING(PartnerId),
		TO_RUST_STRING(UThirdwebRuntimeSettings::GetClientId()),
		TO_RUST_STRING(UThirdwebRuntimeSettings::GetBundleId()),
		nullptr,
		nullptr,
		nullptr,
		TO_RUST_STRING(UThirdwebRuntimeSettings::GetStorageDirectory()),
		TO_RUST_STRING(FString(GetSourceString(Source)))
	).AssignResult(Error))
	{
		Wallet = FInAppWalletHandle(Source, Error);
		Error.Empty();
		return true;
	}
	return false;
}

bool FInAppWalletHandle::IsConnected() const
{
	return Thirdweb::is_connected(ID).GetOutput().ToBool();
}

void FInAppWalletHandle::Disconnect() const
{
	Thirdweb::disconnect(ID).Free();
}

bool FInAppWalletHandle::VerifyOTP(const EThirdwebOTPMethod Method, const FString& OTP, FString& Error)
{
	if (!IsValid())
	{
		Error = TEXT("Invalid wallet handle");
		return false;
	}
	if (Source != Email)
	{
		Error = TEXT("Wallet handle is not email source");
		return false;
	}
	if (UThirdwebRuntimeSettings::IsEcosystem())
	{
		switch (Method)
		{
		case EThirdwebOTPMethod::Phone:
			{
				if (Thirdweb::ecosystem_wallet_verify_otp_phone(ID, TO_RUST_STRING(OTP)).AssignResult(Error, true))
				{
					FThirdwebAnalytics::SendConnectEvent(ToAddress(), GetTypeString());
					return true;
				}
			}
		case EThirdwebOTPMethod::Email:
			{
				if (Thirdweb::ecosystem_wallet_verify_otp_email(ID, TO_RUST_STRING(OTP)).AssignResult(Error, true))
				{
					FThirdwebAnalytics::SendConnectEvent(ToAddress(), GetTypeString());
					return true;
				}
			}
		}
	}
	else
	{
		switch (Method)
		{
		case EThirdwebOTPMethod::Phone:
			{
				if (Thirdweb::in_app_wallet_verify_otp_phone(ID, TO_RUST_STRING(OTP)).AssignResult(Error, true))
				{
					FThirdwebAnalytics::SendConnectEvent(ToAddress(), GetTypeString());
					return true;
				}
			}
		case EThirdwebOTPMethod::Email:
			{
				if (Thirdweb::in_app_wallet_verify_otp_email(ID, TO_RUST_STRING(OTP)).AssignResult(Error, true))
				{
					FThirdwebAnalytics::SendConnectEvent(ToAddress(), GetTypeString());
					return true;
				}
			}
		}
	}

	return false;
}

bool FInAppWalletHandle::SendOTP(const EThirdwebOTPMethod Method, FString& Error)
{
	if (!IsValid())
	{
		Error = TEXT("Invalid wallet handle");
		return false;
	}
	if (Source != Email)
	{
		Error = TEXT("Wallet handle is not email source");
		return false;
	}
	if (UThirdwebRuntimeSettings::IsEcosystem())
	{
		switch (Method)
		{
		case EThirdwebOTPMethod::Phone: return Thirdweb::ecosystem_wallet_send_otp_phone(ID).AssignResult(Error, true);
		case EThirdwebOTPMethod::Email: return Thirdweb::ecosystem_wallet_send_otp_email(ID).AssignResult(Error, true);
		}
	}
	else
	{
		switch (Method)
		{
		case EThirdwebOTPMethod::Phone: return Thirdweb::in_app_wallet_send_otp_phone(ID).AssignResult(Error, true);
		case EThirdwebOTPMethod::Email: return Thirdweb::in_app_wallet_send_otp_email(ID).AssignResult(Error, true);
		}
	}

	return false;
}

bool FInAppWalletHandle::FetchOAuthLoginURL(const FString& RedirectUrl, FString& LoginLink, FString& Error)
{
	if (!IsValid())
	{
		Error = TEXT("Invalid wallet handle");
		return false;
	}
	if (Source != OAuthProvider)
	{
		Error = TEXT("Wallet handle is not OAuth source");
		return false;
	}
	if (UThirdwebRuntimeSettings::IsEcosystem())
	{
		if (Thirdweb::ecosystem_wallet_fetch_oauth_login_link(ID, TO_RUST_STRING(RedirectUrl)).AssignResult(Error))
		{
			LoginLink = Error;
			Error.Empty();
			return true;
		}
	}
	else
	{
		if (Thirdweb::in_app_wallet_fetch_oauth_login_link(ID, TO_RUST_STRING(RedirectUrl)).AssignResult(Error))
		{
			LoginLink = Error;
			Error.Empty();
			return true;
		}
	}

	return false;
}

bool FInAppWalletHandle::SignInWithOAuth(const FString& AuthResult, FString& Error)
{
	if (!IsValid())
	{
		Error = TEXT("Invalid wallet handle");
		return false;
	}
	if (Source != OAuthProvider)
	{
		Error = TEXT("Wallet handle is not OAuth source");
		return false;
	}
	FString Result = AuthResult;
	if (Result.StartsWith(TEXT("%7B%22")))
	{
		Result = FGenericPlatformHttp::UrlDecode(AuthResult);
	}
	if (UThirdwebRuntimeSettings::IsEcosystem())
	{
		if (Thirdweb::ecosystem_wallet_sign_in_with_oauth(ID, TO_RUST_STRING(Result)).AssignResult(Error, true))
		{
			FThirdwebAnalytics::SendConnectEvent(ToAddress(), GetTypeString());
			return true;
		}
	}
	else
	{
		if (Thirdweb::in_app_wallet_sign_in_with_oauth(ID, TO_RUST_STRING(Result)).AssignResult(Error, true))
		{
			FThirdwebAnalytics::SendConnectEvent(ToAddress(), GetTypeString());
			return true;
		}
	}

	return false;
}

bool FInAppWalletHandle::SignInWithJwt(const FString& Jwt, FString& Error)
{
	if (!IsValid())
	{
		Error = TEXT("Invalid wallet handle");
		return false;
	}
	if (Source != EInAppSource::Jwt)
	{
		Error = TEXT("Wallet handle is not JWT source");
		return false;
	}
	if (UThirdwebRuntimeSettings::IsEcosystem())
	{
		if (Thirdweb::ecosystem_wallet_sign_in_with_jwt(ID, TO_RUST_STRING(Jwt)).AssignResult(Error, true))
		{
			FThirdwebAnalytics::SendConnectEvent(ToAddress(), GetTypeString());
			return true;
		}
	}
	else
	{
		if (UThirdwebRuntimeSettings::GetEncryptionKey().IsEmpty())
		{
			Error = TEXT("No encryption key set");
		}
		else
		{
			if (Thirdweb::in_app_wallet_sign_in_with_jwt(ID, TO_RUST_STRING(Jwt), TO_RUST_STRING(UThirdwebRuntimeSettings::GetEncryptionKey())).AssignResult(Error, true))
			{
				FThirdwebAnalytics::SendConnectEvent(ToAddress(), GetTypeString());
				return true;
			}
		}
	}

	return false;
}

bool FInAppWalletHandle::SignInWithAuthEndpoint(const FString& Payload, FString& Error)
{
	if (!IsValid())
	{
		Error = TEXT("Invalid wallet handle");
		return false;
	}
	if (Source != AuthEndpoint)
	{
		Error = TEXT("Wallet handle is not auth endpoint source");
		return false;
	}
	if (UThirdwebRuntimeSettings::IsEcosystem())
	{
		if (Thirdweb::ecosystem_wallet_sign_in_with_auth_endpoint(ID, TO_RUST_STRING(Payload)).AssignResult(Error, true))
		{
			FThirdwebAnalytics::SendConnectEvent(ToAddress(), GetTypeString());
			return true;
		}
	}
	else
	{
		if (UThirdwebRuntimeSettings::GetEncryptionKey().IsEmpty())
		{
			Error = TEXT("No encryption key set");
		} else
		{
			if (Thirdweb::in_app_wallet_sign_in_with_auth_endpoint(ID, TO_RUST_STRING(Payload), TO_RUST_STRING(UThirdwebRuntimeSettings::GetEncryptionKey())).AssignResult(Error, true))
			{
				FThirdwebAnalytics::SendConnectEvent(ToAddress(), GetTypeString());
				return true;
			}
		}
	}
	return false;
}

bool FInAppWalletHandle::SignInWithGuest(FString& Error)
{
	if (!IsValid())
	{
		Error = TEXT("Invalid wallet handle");
		return false;
	}
	if (Source != Guest)
	{
		Error = TEXT("Wallet handle is not guest source");
		return false;
	}
	if (UThirdwebRuntimeSettings::IsEcosystem())
	{
		if (Thirdweb::ecosystem_wallet_sign_in_with_guest(ID, TO_RUST_STRING(FPlatformMisc::GetLoginId())).AssignResult(Error, true))
		{
			FThirdwebAnalytics::SendConnectEvent(ToAddress(), GetTypeString());
			return true;
		}
	}
	else
	{
		if (Thirdweb::in_app_wallet_sign_in_with_guest(ID, TO_RUST_STRING(FPlatformMisc::GetLoginId())).AssignResult(Error, true))
		{
			FThirdwebAnalytics::SendConnectEvent(ToAddress(), GetTypeString());
			return true;
		}
	}
	return false;
}

FString FInAppWalletHandle::GetDisplayName() const
{
	return IsValid()
		       ? FString::Printf(
			       TEXT("%sWallet::%s::%lld"),
			       GetTypeString(),
			       Source == OAuthProvider
				       ? *FString::Printf(TEXT("%s::%s"), GetSourceString(), *ThirdwebUtils::ToString(Provider))
				       : GetSourceString(), ID)
		       : TEXT("InAppWallet::INVALID");
}
