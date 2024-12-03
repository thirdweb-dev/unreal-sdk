// Copyright (c) 2024 Thirdweb. All Rights Reserved.

// ReSharper disable CppMemberFunctionMayBeConst

#include "Wallets/ThirdwebInAppWalletHandle.h"

#include "Thirdweb.h"
#include "ThirdwebCommon.h"
#include "ThirdwebLog.h"
#include "ThirdwebMacros.h"
#include "ThirdwebRuntimeSettings.h"
#include "ThirdwebUtils.h"
#include "Containers/ThirdwebLinkedAccount.h"
#include "Misc/DefaultValueHelper.h"
#include "Tasks/Task.h"

#define CHECK_ECOSYSTEM(ErrorDelegate) \
	if (UThirdwebRuntimeSettings::GetEcosystemId().IsEmpty()) \
	{ \
		TW_LOG(Error, TEXT("Ecosystem ID not set in settings")) \
		if (ErrorDelegate.IsBound()) \
		{ \
			ErrorDelegate.Execute(TEXT("Ecosystem ID not set in settings")); \
		} \
		return; \
	}

#define CHECK_SOURCE(InSource, ErrorDelegate) \
	if (Source != InSource) \
	{ \
		ErrorDelegate.Execute(FString::Printf(TEXT("Wallet handle is not %s source"), GetSourceString(InSource))); \
		return; \
	}

#define CHECK_WALLET_SOURCE(InWallet, InSource, ErrorDelegate) \
	if (InWallet.GetSource() != InSource) \
	{ \
		ErrorDelegate.Execute(FString::Printf(TEXT("Wallet handle is not %s source"), GetSourceString(InSource))); \
		return; \
	}

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

void FInAppWalletHandle::CreateEmailWallet(const FString& Email, const FCreateInAppWalletDelegate& SuccessDelegate, const FStringDelegate& ErrorDelegate)
{
	CHECK_DELEGATES(SuccessDelegate, ErrorDelegate)

	UE::Tasks::Launch(UE_SOURCE_LOCATION, [Email, SuccessDelegate, ErrorDelegate]
	{
		FString Error;
		if (UThirdwebRuntimeSettings::IsEcosystem())
		{
			if (Thirdweb::create_ecosystem_wallet(
				TO_RUST_STRING(UThirdwebRuntimeSettings::GetEcosystemId()),
				TO_RUST_STRING(UThirdwebRuntimeSettings::GetPartnerId()),
				TO_RUST_STRING(UThirdwebRuntimeSettings::GetClientId()),
				TO_RUST_STRING(UThirdwebRuntimeSettings::GetBundleId()),
				nullptr,
				TO_RUST_STRING(Email),
				nullptr,
				TO_RUST_STRING(UThirdwebRuntimeSettings::GetStorageDirectory()),
				nullptr
			).AssignResult(Error))
			{
				SuccessDelegate.Execute(FInAppWalletHandle(EInAppSource::Email, Error));
				return;
			}
		}
		else
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
				SuccessDelegate.Execute(FInAppWalletHandle(EInAppSource::Email, Error));
				return;
			}
		}
		ErrorDelegate.Execute(Error);
	});
}

void FInAppWalletHandle::CreateOAuthWallet(const EThirdwebOAuthProvider Provider, const FCreateInAppWalletDelegate& SuccessDelegate, const FStringDelegate& ErrorDelegate)
{
	CHECK_DELEGATES(SuccessDelegate, ErrorDelegate)
	UE::Tasks::Launch(UE_SOURCE_LOCATION, [Provider, SuccessDelegate, ErrorDelegate]
	{
		FString Error;
		if (UThirdwebRuntimeSettings::IsEcosystem())
		{
			if (Thirdweb::create_ecosystem_wallet(
				TO_RUST_STRING(UThirdwebRuntimeSettings::GetEcosystemId()),
				TO_RUST_STRING(UThirdwebRuntimeSettings::GetPartnerId()),
				TO_RUST_STRING(UThirdwebRuntimeSettings::GetClientId()),
				TO_RUST_STRING(UThirdwebRuntimeSettings::GetBundleId()),
				nullptr,
				nullptr,
				nullptr,
				TO_RUST_STRING(UThirdwebRuntimeSettings::GetStorageDirectory()),
				TO_RUST_STRING(ThirdwebUtils::ToString(Provider))
			).AssignResult(Error))
			{
				SuccessDelegate.Execute(FInAppWalletHandle(Provider, Error));
				return;
			}
		}
		else
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
				SuccessDelegate.Execute(FInAppWalletHandle(Provider, Error));
				return;
			}
		}
		ErrorDelegate.Execute(Error);
	});
}

void FInAppWalletHandle::CreatePhoneWallet(const FString& Phone, const FCreateInAppWalletDelegate& SuccessDelegate, const FStringDelegate& ErrorDelegate)
{
	CHECK_DELEGATES(SuccessDelegate, ErrorDelegate)
	UE::Tasks::Launch(UE_SOURCE_LOCATION, [Phone, SuccessDelegate, ErrorDelegate]
	{
		FString Error;
		if (UThirdwebRuntimeSettings::IsEcosystem())
		{
			if (Thirdweb::create_ecosystem_wallet(
				TO_RUST_STRING(UThirdwebRuntimeSettings::GetEcosystemId()),
				TO_RUST_STRING(UThirdwebRuntimeSettings::GetPartnerId()),
				TO_RUST_STRING(UThirdwebRuntimeSettings::GetClientId()),
				TO_RUST_STRING(UThirdwebRuntimeSettings::GetBundleId()),
				nullptr,
				nullptr,
				TO_RUST_STRING(Phone),
				TO_RUST_STRING(UThirdwebRuntimeSettings::GetStorageDirectory()),
				nullptr
			).AssignResult(Error))
			{
				SuccessDelegate.Execute(FInAppWalletHandle(EInAppSource::Phone, Error));
				return;
			}
		}
		else
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
				SuccessDelegate.Execute(FInAppWalletHandle(EInAppSource::Phone, Error));
				return;
			}
		}
		ErrorDelegate.Execute(Error);
	});
}

void FInAppWalletHandle::CreateSiweWallet(const FCreateInAppWalletDelegate& SuccessDelegate, const FStringDelegate& ErrorDelegate)
{
	CHECK_DELEGATES(SuccessDelegate, ErrorDelegate)
	UE::Tasks::Launch(UE_SOURCE_LOCATION, [SuccessDelegate, ErrorDelegate]
	{
		static FString Provider = TEXT("SIWE");
		FString Error;
		if (UThirdwebRuntimeSettings::IsEcosystem())
		{
			if (Thirdweb::create_ecosystem_wallet(
				TO_RUST_STRING(UThirdwebRuntimeSettings::GetEcosystemId()),
				TO_RUST_STRING(UThirdwebRuntimeSettings::GetPartnerId()),
				TO_RUST_STRING(UThirdwebRuntimeSettings::GetClientId()),
				TO_RUST_STRING(UThirdwebRuntimeSettings::GetBundleId()),
				nullptr,
				nullptr,
				nullptr,
				TO_RUST_STRING(UThirdwebRuntimeSettings::GetStorageDirectory()),
				TO_RUST_STRING(Provider)
			).AssignResult(Error))
			{
				SuccessDelegate.Execute(FInAppWalletHandle(Siwe, Error));
				return;
			}
		}
		else
		{
			if (Thirdweb::create_in_app_wallet(
				TO_RUST_STRING(UThirdwebRuntimeSettings::GetClientId()),
				TO_RUST_STRING(UThirdwebRuntimeSettings::GetBundleId()),
				nullptr,
				nullptr,
				nullptr,
				TO_RUST_STRING(UThirdwebRuntimeSettings::GetStorageDirectory()),
				TO_RUST_STRING(Provider)
			).AssignResult(Error))
			{
				SuccessDelegate.Execute(FInAppWalletHandle(Siwe, Error));
				return;
			}
		}
		ErrorDelegate.Execute(Error);
	});
}

void FInAppWalletHandle::CreateCustomAuthWallet(const EInAppSource Source, const FCreateInAppWalletDelegate& SuccessDelegate, const FStringDelegate& ErrorDelegate)
{
	CHECK_DELEGATES(SuccessDelegate, ErrorDelegate)
	if (Source != Jwt && Source != AuthEndpoint && Source != Guest)
	{
		if (ErrorDelegate.IsBound())
		{
			ErrorDelegate.Execute(TEXT("Invalid custom auth source"));
		}
		return;
	}
	UE::Tasks::Launch(UE_SOURCE_LOCATION, [Source, SuccessDelegate, ErrorDelegate]
	{
		FString Error;
		if (UThirdwebRuntimeSettings::IsEcosystem())
		{
			if (Thirdweb::create_ecosystem_wallet(
				TO_RUST_STRING(UThirdwebRuntimeSettings::GetEcosystemId()),
				TO_RUST_STRING(UThirdwebRuntimeSettings::GetPartnerId()),
				TO_RUST_STRING(UThirdwebRuntimeSettings::GetClientId()),
				TO_RUST_STRING(UThirdwebRuntimeSettings::GetBundleId()),
				nullptr,
				nullptr,
				nullptr,
				TO_RUST_STRING(UThirdwebRuntimeSettings::GetStorageDirectory()),
				TO_RUST_STRING(FString(GetSourceString(Source)))
			).AssignResult(Error))
			{
				SuccessDelegate.Execute(FInAppWalletHandle(Source, Error));
				return;
			}
		}
		else
		{
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
				SuccessDelegate.Execute(FInAppWalletHandle(Source, Error));
				return;
			}
		}
		ErrorDelegate.Execute(Error);
	});
}

bool FInAppWalletHandle::IsConnected() const
{
	return Thirdweb::is_connected(ID).GetOutput().ToBool();
}

void FInAppWalletHandle::Disconnect() const
{
	Thirdweb::disconnect(ID).Free();
}

void FInAppWalletHandle::SendOTP(const FStreamableDelegate& SuccessDelegate, const FStringDelegate& ErrorDelegate)
{
	CHECK_DELEGATES(SuccessDelegate, ErrorDelegate)
	CHECK_VALIDITY(ErrorDelegate)
	FInAppWalletHandle ThisCopy = *this;
	UE::Tasks::Launch(UE_SOURCE_LOCATION, [ThisCopy, SuccessDelegate, ErrorDelegate]
	{
		FString Error;
		if (UThirdwebRuntimeSettings::IsEcosystem())
		{
			switch (ThisCopy.GetSource())
			{
			case Phone:
				{
					if (Thirdweb::ecosystem_wallet_send_otp_phone(ThisCopy.GetID()).AssignResult(Error, true))
					{
						SuccessDelegate.Execute();
						return;
					}
					break;
				}
			case Email:
				{
					if (Thirdweb::ecosystem_wallet_send_otp_email(ThisCopy.GetID()).AssignResult(Error, true))
					{
						SuccessDelegate.Execute();
						return;
					}
					break;
				}
			default:
				{
					ErrorDelegate.Execute(TEXT("Wallet handle is not email/phone source"));
					return;
				}
			}
		}
		else
		{
			switch (ThisCopy.GetSource())
			{
			case Phone:
				{
					if (Thirdweb::in_app_wallet_send_otp_phone(ThisCopy.GetID()).AssignResult(Error, true))
					{
						SuccessDelegate.Execute();
						return;
					}
					break;
				}
			case Email:
				{
					if (Thirdweb::in_app_wallet_send_otp_email(ThisCopy.GetID()).AssignResult(Error, true))
					{
						SuccessDelegate.Execute();
						return;
					}
					break;
				}
			default:
				{
					ErrorDelegate.Execute(TEXT("Wallet handle is not email/phone source"));
					return;
				}
			}
		}
		ErrorDelegate.Execute(Error);
	});
}

void FInAppWalletHandle::SignInWithOTP(const FString& OTP, const FStreamableDelegate& SuccessDelegate, const FStringDelegate& ErrorDelegate)
{
	CHECK_DELEGATES(SuccessDelegate, ErrorDelegate)
	CHECK_VALIDITY(ErrorDelegate)
	if (Source != Phone && Source != Email)
	{
		ErrorDelegate.Execute(TEXT("Wallet handle is not email/phone OTP source"));
		return;
	}
	FInAppWalletHandle ThisCopy = *this;
	UE::Tasks::Launch(UE_SOURCE_LOCATION, [ThisCopy, OTP, SuccessDelegate, ErrorDelegate]
	{
		FString Error;
		if (UThirdwebRuntimeSettings::IsEcosystem())
		{
			switch (ThisCopy.GetSource())
			{
			case Phone:
				{
					if (Thirdweb::ecosystem_wallet_sign_in_with_otp_phone(ThisCopy.GetID(), TO_RUST_STRING(OTP)).AssignResult(Error, true))
					{
						ThirdwebUtils::Internal::SendConnectEvent(ThisCopy);
						SuccessDelegate.Execute();
						return;
					}
					break;
				}
			case Email:
				{
					if (Thirdweb::ecosystem_wallet_sign_in_with_otp_email(ThisCopy.GetID(), TO_RUST_STRING(OTP)).AssignResult(Error, true))
					{
						ThirdwebUtils::Internal::SendConnectEvent(ThisCopy);
						SuccessDelegate.Execute();
						return;
					}
					break;
				}
			default: return;
			}
		}
		else
		{
			switch (ThisCopy.GetSource())
			{
			case Phone:
				{
					if (Thirdweb::in_app_wallet_sign_in_with_otp_phone(ThisCopy.GetID(), TO_RUST_STRING(OTP)).AssignResult(Error, true))
					{
						ThirdwebUtils::Internal::SendConnectEvent(ThisCopy);
						SuccessDelegate.Execute();
						return;
					}
				}
			case Email:
				{
					if (Thirdweb::in_app_wallet_sign_in_with_otp_email(ThisCopy.GetID(), TO_RUST_STRING(OTP)).AssignResult(Error, true))
					{
						ThirdwebUtils::Internal::SendConnectEvent(ThisCopy);
						SuccessDelegate.Execute();
						return;
					}
				}
			default: return;
			}
		}
		ErrorDelegate.Execute(Error);
	});
}

void FInAppWalletHandle::LinkOTP(const FInAppWalletHandle& Wallet, const FString& OTP, const FStreamableDelegate& SuccessDelegate, const FStringDelegate& ErrorDelegate)
{
	CHECK_DELEGATES(SuccessDelegate, ErrorDelegate)
	CHECK_VALIDITY(ErrorDelegate)
	CHECK_ECOSYSTEM(ErrorDelegate)
	if (Wallet.GetSource() != Phone && Wallet.GetSource() != Email)
	{
		ErrorDelegate.Execute(TEXT("Wallet handle is not email/phone OTP source"));
		return;
	}
	FInAppWalletHandle ThisCopy = *this;
	UE::Tasks::Launch(UE_SOURCE_LOCATION, [ThisCopy, Wallet, OTP, SuccessDelegate, ErrorDelegate]
	{
		FString Error;
		switch (ThisCopy.GetSource())
		{
		case Phone:
			{
				if (Thirdweb::ecosystem_wallet_link_account(ThisCopy.GetID(), Wallet.GetID(), TO_RUST_STRING(OTP), nullptr, nullptr, nullptr, nullptr, nullptr, nullptr).AssignResult(Error, true))
				{
					SuccessDelegate.Execute();
					return;
				}
				break;
			}
		case Email:
			{
				if (Thirdweb::ecosystem_wallet_sign_in_with_otp_email(ThisCopy.GetID(), TO_RUST_STRING(OTP)).AssignResult(Error, true))
				{
					SuccessDelegate.Execute();
					return;
				}
				break;
			}
		default: return;
		}
		ErrorDelegate.Execute(Error);
	});
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
	FInAppWalletHandle ThisCopy = *this;
	if (UThirdwebRuntimeSettings::IsEcosystem())
	{
		if (Thirdweb::ecosystem_wallet_fetch_oauth_login_link(ThisCopy.GetID(), TO_RUST_STRING(RedirectUrl)).AssignResult(Error))
		{
			LoginLink = Error;
			Error.Empty();
			return true;
		}
	}
	else
	{
		if (Thirdweb::in_app_wallet_fetch_oauth_login_link(ThisCopy.GetID(), TO_RUST_STRING(RedirectUrl)).AssignResult(Error))
		{
			LoginLink = Error;
			Error.Empty();
			return true;
		}
	}

	return false;
}

void FInAppWalletHandle::SignInWithOAuth(const FString& AuthResult, const FStreamableDelegate& SuccessDelegate, const FStringDelegate& ErrorDelegate)
{
	CHECK_DELEGATES(SuccessDelegate, ErrorDelegate)
	CHECK_VALIDITY(ErrorDelegate)
	CHECK_SOURCE(OAuthProvider, ErrorDelegate)

	FString Result = ThirdwebUtils::ParseAuthResult(AuthResult);
	FInAppWalletHandle ThisCopy = *this;
	UE::Tasks::Launch(UE_SOURCE_LOCATION, [ThisCopy, Result, SuccessDelegate, ErrorDelegate]
	{
		FString Error;
		if (UThirdwebRuntimeSettings::IsEcosystem())
		{
			TW_LOG(VeryVerbose, TEXT("FInAppWalletHandle::SignInWithOAuth::Task::%s"), *Result)
			if (Thirdweb::ecosystem_wallet_sign_in_with_oauth(ThisCopy.GetID(), TO_RUST_STRING(Result)).AssignResult(Error, true))
			{
				ThirdwebUtils::Internal::SendConnectEvent(ThisCopy);
				SuccessDelegate.Execute();
				return;
			}
		}
		else
		{
			if (Thirdweb::in_app_wallet_sign_in_with_oauth(ThisCopy.GetID(), TO_RUST_STRING(Result)).AssignResult(Error, true))
			{
				ThirdwebUtils::Internal::SendConnectEvent(ThisCopy);
				SuccessDelegate.Execute();
				return;
			}
		}
		ErrorDelegate.Execute(Error);
	});
}

void FInAppWalletHandle::LinkOAuth(const FInAppWalletHandle& Wallet, const FString& AuthResult, const FStreamableDelegate& SuccessDelegate, const FStringDelegate& ErrorDelegate)
{
	CHECK_DELEGATES(SuccessDelegate, ErrorDelegate)
	CHECK_VALIDITY(ErrorDelegate)
	CHECK_ECOSYSTEM(ErrorDelegate)
	CHECK_WALLET_SOURCE(Wallet, OAuthProvider, ErrorDelegate)

	FString Result = ThirdwebUtils::ParseAuthResult(AuthResult);
	FInAppWalletHandle ThisCopy = *this;
	UE::Tasks::Launch(UE_SOURCE_LOCATION, [ThisCopy, Wallet, Result, SuccessDelegate, ErrorDelegate]
	{
		FString Error;
		if (Thirdweb::ecosystem_wallet_link_account(ThisCopy.GetID(), Wallet.GetID(), nullptr, TO_RUST_STRING(Result), nullptr, nullptr, nullptr, nullptr, nullptr).AssignResult(Error, true))
		{
			SuccessDelegate.Execute();
			return;
		}
		ErrorDelegate.Execute(Error);
	});
}

void FInAppWalletHandle::SignInWithJwt(const FString& Jwt, const FStreamableDelegate& SuccessDelegate, const FStringDelegate& ErrorDelegate)
{
	CHECK_DELEGATES(SuccessDelegate, ErrorDelegate)
	CHECK_VALIDITY(ErrorDelegate)
	CHECK_SOURCE(EInAppSource::Jwt, ErrorDelegate)

	FInAppWalletHandle ThisCopy = *this;
	UE::Tasks::Launch(UE_SOURCE_LOCATION, [ThisCopy, Jwt, SuccessDelegate, ErrorDelegate]
	{
		FString Error;
		if (UThirdwebRuntimeSettings::IsEcosystem())
		{
			if (Thirdweb::ecosystem_wallet_sign_in_with_jwt(ThisCopy.GetID(), TO_RUST_STRING(Jwt)).AssignResult(Error, true))
			{
				ThirdwebUtils::Internal::SendConnectEvent(ThisCopy);
				SuccessDelegate.Execute();
				return;
			}
		}
		else
		{
			if (UThirdwebRuntimeSettings::GetEncryptionKey().IsEmpty())
			{
				ErrorDelegate.Execute(TEXT("No encryption key set"));
				return;
			}
			if (Thirdweb::in_app_wallet_sign_in_with_jwt(ThisCopy.GetID(), TO_RUST_STRING(Jwt), TO_RUST_STRING(UThirdwebRuntimeSettings::GetEncryptionKey())).AssignResult(Error, true))
			{
				ThirdwebUtils::Internal::SendConnectEvent(ThisCopy);
				SuccessDelegate.Execute();
				return;
			}
		}
		ErrorDelegate.Execute(Error);
	});
}

void FInAppWalletHandle::LinkJwt(const FInAppWalletHandle& Wallet, const FString& Jwt, const FStreamableDelegate& SuccessDelegate, const FStringDelegate& ErrorDelegate)
{
	CHECK_DELEGATES(SuccessDelegate, ErrorDelegate)
	CHECK_VALIDITY(ErrorDelegate)
	CHECK_ECOSYSTEM(ErrorDelegate)
	CHECK_WALLET_SOURCE(Wallet, EInAppSource::Jwt, ErrorDelegate)

	FInAppWalletHandle ThisCopy = *this;
	UE::Tasks::Launch(UE_SOURCE_LOCATION, [ThisCopy, Wallet, Jwt, SuccessDelegate, ErrorDelegate]
	{
		FString Error;
		if (Thirdweb::ecosystem_wallet_link_account(ThisCopy.GetID(), Wallet.GetID(), nullptr, nullptr, TO_RUST_STRING(Jwt), nullptr, nullptr, nullptr, nullptr).AssignResult(Error, true))
		{
			SuccessDelegate.Execute();
			return;
		}
		ErrorDelegate.Execute(Error);
	});
}

void FInAppWalletHandle::SignInWithAuthEndpoint(const FString& Payload, const FStreamableDelegate& SuccessDelegate, const FStringDelegate& ErrorDelegate)
{
	CHECK_DELEGATES(SuccessDelegate, ErrorDelegate)
	CHECK_VALIDITY(ErrorDelegate)
	CHECK_SOURCE(AuthEndpoint, ErrorDelegate)

	FInAppWalletHandle ThisCopy = *this;
	UE::Tasks::Launch(UE_SOURCE_LOCATION, [ThisCopy, Payload, SuccessDelegate, ErrorDelegate]
	{
		FString Error;
		if (UThirdwebRuntimeSettings::IsEcosystem())
		{
			if (Thirdweb::ecosystem_wallet_sign_in_with_auth_endpoint(ThisCopy.GetID(), TO_RUST_STRING(Payload)).AssignResult(Error, true))
			{
				ThirdwebUtils::Internal::SendConnectEvent(ThisCopy);
				SuccessDelegate.Execute();
				return;
			}
		}
		else
		{
			if (UThirdwebRuntimeSettings::GetEncryptionKey().IsEmpty())
			{
				ErrorDelegate.Execute(TEXT("No encryption key set"));
				return;
			}
			if (Thirdweb::in_app_wallet_sign_in_with_auth_endpoint(ThisCopy.GetID(), TO_RUST_STRING(Payload), TO_RUST_STRING(UThirdwebRuntimeSettings::GetEncryptionKey())).AssignResult(Error, true))
			{
				ThirdwebUtils::Internal::SendConnectEvent(ThisCopy);
				SuccessDelegate.Execute();
				return;
			}
		}
		ErrorDelegate.Execute(Error);
	});
}

void FInAppWalletHandle::LinkAuthEndpoint(const FInAppWalletHandle& Wallet, const FString& Payload, const FStreamableDelegate& SuccessDelegate, const FStringDelegate& ErrorDelegate)
{
	CHECK_DELEGATES(SuccessDelegate, ErrorDelegate)
	CHECK_VALIDITY(ErrorDelegate)
	CHECK_ECOSYSTEM(ErrorDelegate)
	CHECK_WALLET_SOURCE(Wallet, AuthEndpoint, ErrorDelegate)

	FInAppWalletHandle ThisCopy = *this;
	UE::Tasks::Launch(UE_SOURCE_LOCATION, [ThisCopy, Wallet, Payload, SuccessDelegate, ErrorDelegate]
	{
		FString Error;
		if (Thirdweb::ecosystem_wallet_link_account(ThisCopy.GetID(), Wallet.GetID(), nullptr, nullptr, nullptr, TO_RUST_STRING(Payload), nullptr, nullptr, nullptr).AssignResult(Error, true))
		{
			SuccessDelegate.Execute();
			return;
		}
		ErrorDelegate.Execute(Error);
	});
}

void FInAppWalletHandle::SignInWithGuest(const FStreamableDelegate& SuccessDelegate, const FStringDelegate& ErrorDelegate)
{
	CHECK_DELEGATES(SuccessDelegate, ErrorDelegate)
	CHECK_VALIDITY(ErrorDelegate)
	CHECK_SOURCE(Guest, ErrorDelegate)

	FInAppWalletHandle ThisCopy = *this;
	UE::Tasks::Launch(UE_SOURCE_LOCATION, [ThisCopy, SuccessDelegate, ErrorDelegate]
	{
		FString Error;
		if (UThirdwebRuntimeSettings::IsEcosystem())
		{
			if (Thirdweb::ecosystem_wallet_sign_in_with_guest(ThisCopy.GetID(), TO_RUST_STRING(FPlatformMisc::GetLoginId())).AssignResult(Error, true))
			{
				ThirdwebUtils::Internal::SendConnectEvent(ThisCopy);
				SuccessDelegate.Execute();
				return;
			}
		}
		else
		{
			if (Thirdweb::in_app_wallet_sign_in_with_guest(ThisCopy.GetID(), TO_RUST_STRING(FPlatformMisc::GetLoginId())).AssignResult(Error, true))
			{
				ThirdwebUtils::Internal::SendConnectEvent(ThisCopy);
				SuccessDelegate.Execute();
				return;
			}
		}
		ErrorDelegate.Execute(Error);
	});
}

void FInAppWalletHandle::LinkGuest(const FInAppWalletHandle& Wallet, const FStreamableDelegate& SuccessDelegate, const FStringDelegate& ErrorDelegate)
{
	CHECK_DELEGATES(SuccessDelegate, ErrorDelegate)
	CHECK_VALIDITY(ErrorDelegate)
	CHECK_ECOSYSTEM(ErrorDelegate)
	CHECK_WALLET_SOURCE(Wallet, Guest, ErrorDelegate)

	FInAppWalletHandle ThisCopy = *this;
	UE::Tasks::Launch(UE_SOURCE_LOCATION, [ThisCopy, Wallet, SuccessDelegate, ErrorDelegate]
	{
		FString Error;
		if (Thirdweb::ecosystem_wallet_link_account(ThisCopy.GetID(), Wallet.GetID(), nullptr, nullptr, nullptr, nullptr, TO_RUST_STRING(FPlatformMisc::GetLoginId()), nullptr, nullptr).
			AssignResult(Error, true))
		{
			SuccessDelegate.Execute();
			return;
		}
		ErrorDelegate.Execute(Error);
	});
}

void FInAppWalletHandle::SignInWithEthereum(const FString& Payload, const FString& Signature, const FStreamableDelegate& SuccessDelegate, const FStringDelegate& ErrorDelegate)
{
	CHECK_DELEGATES(SuccessDelegate, ErrorDelegate)
	CHECK_VALIDITY(ErrorDelegate)
	CHECK_SOURCE(Siwe, ErrorDelegate)

	FInAppWalletHandle ThisCopy = *this;
	UE::Tasks::Launch(UE_SOURCE_LOCATION, [ThisCopy, Payload, Signature, SuccessDelegate, ErrorDelegate]
	{
		UE_LOG(LogTemp, VeryVerbose, TEXT("FInAppWalletHandle::SignInWithEthereum::Task::WalletHandle=%lld | Signature=%s | Payload=%s"), ThisCopy.GetID(), *Signature, *Payload);
		FString Error;
		if (UThirdwebRuntimeSettings::IsEcosystem())
		{
			if (Thirdweb::ecosystem_wallet_sign_in_with_siwe(ThisCopy.GetID(), TO_RUST_STRING(Payload), TO_RUST_STRING(Signature)).AssignResult(Error, true))
			{
				ThirdwebUtils::Internal::SendConnectEvent(ThisCopy);
				SuccessDelegate.Execute();
				return;
			}
		}
		else
		{
			if (Thirdweb::in_app_wallet_sign_in_with_siwe(ThisCopy.GetID(), TO_RUST_STRING(Payload), TO_RUST_STRING(Signature)).AssignResult(Error, true))
			{
				ThirdwebUtils::Internal::SendConnectEvent(ThisCopy);
				SuccessDelegate.Execute();
				return;
			}
		}
		ErrorDelegate.Execute(Error);
	});
}

void FInAppWalletHandle::LinkSiwe(const FInAppWalletHandle& Wallet, const FString& Payload, const FString& Signature, const FStreamableDelegate& SuccessDelegate, const FStringDelegate& ErrorDelegate)
{
	CHECK_DELEGATES(SuccessDelegate, ErrorDelegate)
	CHECK_VALIDITY(ErrorDelegate)
	CHECK_ECOSYSTEM(ErrorDelegate)
	CHECK_WALLET_SOURCE(Wallet, Siwe, ErrorDelegate)

	FInAppWalletHandle ThisCopy = *this;
	UE::Tasks::Launch(UE_SOURCE_LOCATION, [ThisCopy, Wallet, Payload, Signature, SuccessDelegate, ErrorDelegate]
	{
		FString Error;
		if (Thirdweb::ecosystem_wallet_link_account(ThisCopy.GetID(), Wallet.GetID(), nullptr, nullptr, nullptr, nullptr, nullptr, TO_RUST_STRING(Signature), TO_RUST_STRING(Payload)).
			AssignResult(Error, true))
		{
			SuccessDelegate.Execute();
			return;
		}
		ErrorDelegate.Execute(Error);
	});
}

void FInAppWalletHandle::GetLinkedAccounts(const FGetLinkedAccountsDelegate& SuccessDelegate, const FStringDelegate& ErrorDelegate)
{
	CHECK_DELEGATES(SuccessDelegate, ErrorDelegate)
	CHECK_VALIDITY(ErrorDelegate)
	CHECK_ECOSYSTEM(ErrorDelegate)

	FInAppWalletHandle ThisCopy = *this;
	UE::Tasks::Launch(UE_SOURCE_LOCATION, [ThisCopy, SuccessDelegate, ErrorDelegate]
	{
		FString Output;
		if (Thirdweb::ecosystem_wallet_get_linked_accounts(ThisCopy.GetID()).AssignResult(Output))
		{
			TArray<FThirdwebLinkedAccount> LinkedAccounts;
			TArray<TSharedPtr<FJsonValue>> JsonValueArray = ThirdwebUtils::Json::ToJsonArray(Output);
			for (int i = 0; i < JsonValueArray.Num(); i++)
			{
				if (JsonValueArray[i]->Type == EJson::Object)
				{
					LinkedAccounts.Emplace(FThirdwebLinkedAccount::FromJson(JsonValueArray[i]->AsObject()));
				}
			}
			SuccessDelegate.Execute(LinkedAccounts);
			return;
		}
		ErrorDelegate.Execute(Output);
	});
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
