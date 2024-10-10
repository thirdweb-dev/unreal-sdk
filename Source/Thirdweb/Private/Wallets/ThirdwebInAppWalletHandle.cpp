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

#define CHECK_DELEGATES(SuccessDelegate, ErrorDelegate) \
	if (!SuccessDelegate.IsBound()) \
	{ \
		if (ErrorDelegate.IsBound()) \
		{ \
			TW_LOG(Error, TEXT("No SuccessDelegate bound::%s"), UE_SOURCE_LOCATION) \
			ErrorDelegate.Execute(TEXT("Success Delegate Not Bound")); \
		} \
		return; \
	} \
	if (!ErrorDelegate.IsBound()) { \
		TW_LOG(Error, TEXT("No ErrorDelegate bound::%s"), UE_SOURCE_LOCATION) \
	}

#define CHECK_VALIDITY(ErrorDelegate) \
	if (!IsValid()) \
	{ \
		TW_LOG(Error, TEXT("Invalid wallet handle")) \
		if (ErrorDelegate.IsBound()) \
		{ \
			ErrorDelegate.Execute(TEXT("Invalid wallet handle")); \
		} \
		return; \
	}

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

#define CHECK_SOURCE(InSource) \
	if (Source != InSource) \
	{ \
		TW_LOG(Error, TEXT("Wallet handle is not %s source"), FInAppWalletHandle::GetSourceString(InSource)) \
		ErrorDelegate.Execute(FString::Format(TEXT("Wallet handle is not %s source"), FInAppWalletHandle::GetSourceString(InSource))); \
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
		if (FString Error; Thirdweb::create_in_app_wallet(
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
		}
		else
		{
			ErrorDelegate.Execute(Error);
		}
	});
}

void FInAppWalletHandle::CreateEcosystemEmailWallet(const FString& PartnerId, const FString& Email, const FCreateInAppWalletDelegate& SuccessDelegate, const FStringDelegate& ErrorDelegate)
{
	CHECK_DELEGATES(SuccessDelegate, ErrorDelegate)
	CHECK_ECOSYSTEM(ErrorDelegate)
	UE::Tasks::Launch(UE_SOURCE_LOCATION, [PartnerId, Email, SuccessDelegate, ErrorDelegate]
	{
		if (FString Error; Thirdweb::create_ecosystem_wallet(
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
			FInAppWalletHandle Wallet = FInAppWalletHandle(EInAppSource::Email, Error);
			Wallet.EcosystemPartnerId = PartnerId;
			SuccessDelegate.Execute(Wallet);
		}
		else
		{
			ErrorDelegate.Execute(Error);
		}
	});
}

void FInAppWalletHandle::CreateOAuthWallet(const EThirdwebOAuthProvider Provider, const FCreateInAppWalletDelegate& SuccessDelegate, const FStringDelegate& ErrorDelegate)
{
	CHECK_DELEGATES(SuccessDelegate, ErrorDelegate)
	UE::Tasks::Launch(UE_SOURCE_LOCATION, [Provider, SuccessDelegate, ErrorDelegate]
	{
		if (FString Error; Thirdweb::create_in_app_wallet(
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
		}
		else
		{
			ErrorDelegate.Execute(Error);
		}
	});
}

void FInAppWalletHandle::CreateEcosystemOAuthWallet(const FString& PartnerId,
                                                    const EThirdwebOAuthProvider Provider,
                                                    const FCreateInAppWalletDelegate& SuccessDelegate,
                                                    const FStringDelegate& ErrorDelegate)
{
	CHECK_DELEGATES(SuccessDelegate, ErrorDelegate)
	CHECK_ECOSYSTEM(ErrorDelegate)
	UE::Tasks::Launch(UE_SOURCE_LOCATION, [PartnerId, Provider, SuccessDelegate, ErrorDelegate]
	{
		if (FString Error; Thirdweb::create_ecosystem_wallet(
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
			FInAppWalletHandle Wallet = FInAppWalletHandle(Provider, Error);
			Wallet.EcosystemPartnerId = PartnerId;
			SuccessDelegate.Execute(Wallet);
		}
		else
		{
			ErrorDelegate.Execute(Error);
		}
	});
}

void FInAppWalletHandle::CreatePhoneWallet(const FString& Phone, const FCreateInAppWalletDelegate& SuccessDelegate, const FStringDelegate& ErrorDelegate)
{
	CHECK_DELEGATES(SuccessDelegate, ErrorDelegate)
	UE::Tasks::Launch(UE_SOURCE_LOCATION, [Phone, SuccessDelegate, ErrorDelegate]
	{
		if (FString Error; Thirdweb::create_in_app_wallet(
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
		}
		else
		{
			ErrorDelegate.Execute(Error);
		}
	});
}

void FInAppWalletHandle::CreateEcosystemPhoneWallet(const FString& PartnerId, const FString& Phone, const FCreateInAppWalletDelegate& SuccessDelegate, const FStringDelegate& ErrorDelegate)
{
	CHECK_DELEGATES(SuccessDelegate, ErrorDelegate)
	CHECK_ECOSYSTEM(ErrorDelegate)
	UE::Tasks::Launch(UE_SOURCE_LOCATION, [PartnerId, Phone, SuccessDelegate, ErrorDelegate]
	{
		if (FString Error; Thirdweb::create_ecosystem_wallet(
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
			FInAppWalletHandle Wallet = FInAppWalletHandle(EInAppSource::Phone, Error);
			Wallet.EcosystemPartnerId = PartnerId;
			SuccessDelegate.Execute(Wallet);
		}
		else
		{
			ErrorDelegate.Execute(Error);
		}
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
		if (FString Error; Thirdweb::create_in_app_wallet(
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
		}
		else
		{
			ErrorDelegate.Execute(Error);
		}
	});
}

void FInAppWalletHandle::CreateEcosystemCustomAuthWallet(const FString& PartnerId, const EInAppSource Source, const FCreateInAppWalletDelegate& SuccessDelegate, const FStringDelegate& ErrorDelegate)
{
	CHECK_DELEGATES(SuccessDelegate, ErrorDelegate)
	CHECK_ECOSYSTEM(ErrorDelegate)
	if (Source != Jwt && Source != AuthEndpoint && Source != Guest)
	{
		ErrorDelegate.Execute(TEXT("Invalid custom auth source"));
		return;
	}
	UE::Tasks::Launch(UE_SOURCE_LOCATION, [PartnerId, Source, SuccessDelegate, ErrorDelegate]
	{
		if (FString Error; Thirdweb::create_ecosystem_wallet(
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
			FInAppWalletHandle Wallet = FInAppWalletHandle(Source, Error);
			Wallet.EcosystemPartnerId = PartnerId;
			SuccessDelegate.Execute(Wallet);
		}
		else
		{
			ErrorDelegate.Execute(Error);
		}
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
	UE::Tasks::Launch(UE_SOURCE_LOCATION, [this, SuccessDelegate, ErrorDelegate]
	{
		FString Error;
		if (UThirdwebRuntimeSettings::IsEcosystem())
		{
			switch (Source)
			{
			case Phone:
				{
					if (Thirdweb::ecosystem_wallet_send_otp_phone(ID).AssignResult(Error, true))
					{
						SuccessDelegate.Execute();
						return;
					}
					break;
				}
			case Email:
				{
					if (Thirdweb::ecosystem_wallet_send_otp_email(ID).AssignResult(Error, true))
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
			switch (Source)
			{
			case Phone:
				{
					if (Thirdweb::in_app_wallet_send_otp_phone(ID).AssignResult(Error, true))
					{
						SuccessDelegate.Execute();
						return;
					}
					break;
				}
			case Email:
				{
					if (Thirdweb::in_app_wallet_send_otp_email(ID).AssignResult(Error, true))
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
	UE::Tasks::Launch(UE_SOURCE_LOCATION, [this, OTP, SuccessDelegate, ErrorDelegate]
	{
		FString Error;
		if (UThirdwebRuntimeSettings::IsEcosystem())
		{
			switch (Source)
			{
			case Phone:
				{
					if (Thirdweb::ecosystem_wallet_verify_otp_phone(ID, TO_RUST_STRING(OTP)).AssignResult(Error, true))
					{
						FThirdwebAnalytics::SendConnectEvent(ToAddress(), GetTypeString());
						SuccessDelegate.Execute();
						return;
					}
					break;
				}
			case Email:
				{
					if (Thirdweb::ecosystem_wallet_verify_otp_email(ID, TO_RUST_STRING(OTP)).AssignResult(Error, true))
					{
						FThirdwebAnalytics::SendConnectEvent(ToAddress(), GetTypeString());
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
			switch (Source)
			{
			case Phone:
				{
					if (Thirdweb::in_app_wallet_verify_otp_phone(ID, TO_RUST_STRING(OTP)).AssignResult(Error, true))
					{
						FThirdwebAnalytics::SendConnectEvent(ToAddress(), GetTypeString());
						SuccessDelegate.Execute();
						return;
					}
				}
			case Email:
				{
					if (Thirdweb::in_app_wallet_verify_otp_email(ID, TO_RUST_STRING(OTP)).AssignResult(Error, true))
					{
						FThirdwebAnalytics::SendConnectEvent(ToAddress(), GetTypeString());
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
	if (Wallet.GetSource() != Phone && Wallet.GetSource() != Email)
	{
		ErrorDelegate.Execute(TEXT("Wallet handle is not email/phone OTP source"));
		return;
	}
	UE::Tasks::Launch(UE_SOURCE_LOCATION, [this, Wallet, OTP, SuccessDelegate, ErrorDelegate]
	{
		FString Error;
		switch (Source)
		{
		case Phone:
			{
				if (Thirdweb::ecosystem_wallet_link_account(ID, Wallet.GetID(), TO_RUST_STRING(OTP), nullptr, nullptr, nullptr).AssignResult(Error, true))
				{
					SuccessDelegate.Execute();
					return;
				}
				break;
			}
		case Email:
			{
				if (Thirdweb::ecosystem_wallet_verify_otp_email(ID, TO_RUST_STRING(OTP)).AssignResult(Error, true))
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

void FInAppWalletHandle::SignInWithOAuth(const FString& AuthResult, const FStreamableDelegate& SuccessDelegate, const FStringDelegate& ErrorDelegate)
{
	CHECK_DELEGATES(SuccessDelegate, ErrorDelegate)
	CHECK_VALIDITY(ErrorDelegate)
	if (Source != OAuthProvider)
	{
		ErrorDelegate.Execute(TEXT("Wallet handle is not OAuth source"));
		return;
	}
	FString Result = AuthResult;
	if (Result.StartsWith(TEXT("%7B%22")))
	{
		Result = FGenericPlatformHttp::UrlDecode(AuthResult);
	}
	UE::Tasks::Launch(UE_SOURCE_LOCATION, [this, Result, SuccessDelegate, ErrorDelegate]
	{
		FString Error;
		if (UThirdwebRuntimeSettings::IsEcosystem())
		{
			if (Thirdweb::ecosystem_wallet_sign_in_with_oauth(ID, TO_RUST_STRING(Result)).AssignResult(Error, true))
			{
				FThirdwebAnalytics::SendConnectEvent(ToAddress(), GetTypeString());
				SuccessDelegate.Execute();
				return;
			}
		}
		else
		{
			if (Thirdweb::in_app_wallet_sign_in_with_oauth(ID, TO_RUST_STRING(Result)).AssignResult(Error, true))
			{
				FThirdwebAnalytics::SendConnectEvent(ToAddress(), GetTypeString());
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
	if (Wallet.GetSource() != OAuthProvider)
	{
		ErrorDelegate.Execute(TEXT("Wallet handle is not OAuth source"));
		return;
	}
	FString Result = AuthResult;
	if (Result.StartsWith(TEXT("%7B%22")))
	{
		Result = FGenericPlatformHttp::UrlDecode(AuthResult);
	}
	UE::Tasks::Launch(UE_SOURCE_LOCATION, [this, Wallet, Result, SuccessDelegate, ErrorDelegate]
	{
		FString Error;
		if (Thirdweb::ecosystem_wallet_link_account(ID, Wallet.GetID(), nullptr, TO_RUST_STRING(Result), nullptr, nullptr).AssignResult(Error, true))
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
	if (Source != EInAppSource::Jwt)
	{
		ErrorDelegate.Execute(TEXT("Wallet handle is not JWT source"));
		return;
	}
	UE::Tasks::Launch(UE_SOURCE_LOCATION, [this, Jwt, SuccessDelegate, ErrorDelegate]
	{
		FString Error;
		if (UThirdwebRuntimeSettings::IsEcosystem())
		{
			if (Thirdweb::ecosystem_wallet_sign_in_with_jwt(ID, TO_RUST_STRING(Jwt)).AssignResult(Error, true))
			{
				FThirdwebAnalytics::SendConnectEvent(ToAddress(), GetTypeString());
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
			if (Thirdweb::in_app_wallet_sign_in_with_jwt(ID, TO_RUST_STRING(Jwt), TO_RUST_STRING(UThirdwebRuntimeSettings::GetEncryptionKey())).AssignResult(Error, true))
			{
				FThirdwebAnalytics::SendConnectEvent(ToAddress(), GetTypeString());
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
	if (Wallet.GetSource() != EInAppSource::Jwt)
	{
		ErrorDelegate.Execute(TEXT("Wallet handle is not JWT source"));
		return;
	}
	UE::Tasks::Launch(UE_SOURCE_LOCATION, [this, Wallet, Jwt, SuccessDelegate, ErrorDelegate]
	{
		FString Error;
		if (Thirdweb::ecosystem_wallet_link_account(ID, Wallet.GetID(), nullptr, nullptr, TO_RUST_STRING(Jwt), nullptr).AssignResult(Error, true))
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
	if (Source != AuthEndpoint)
	{
		ErrorDelegate.Execute(TEXT("Wallet handle is not auth endpoint source"));
		return;
	}
	UE::Tasks::Launch(UE_SOURCE_LOCATION, [this, Payload, SuccessDelegate, ErrorDelegate]
	{
		FString Error;
		if (UThirdwebRuntimeSettings::IsEcosystem())
		{
			if (Thirdweb::ecosystem_wallet_sign_in_with_auth_endpoint(ID, TO_RUST_STRING(Payload)).AssignResult(Error, true))
			{
				FThirdwebAnalytics::SendConnectEvent(ToAddress(), GetTypeString());
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
			if (Thirdweb::in_app_wallet_sign_in_with_auth_endpoint(ID, TO_RUST_STRING(Payload), TO_RUST_STRING(UThirdwebRuntimeSettings::GetEncryptionKey())).AssignResult(Error, true))
			{
				FThirdwebAnalytics::SendConnectEvent(ToAddress(), GetTypeString());
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
	if (Wallet.GetSource() != AuthEndpoint)
	{
		ErrorDelegate.Execute(TEXT("Wallet handle is not auth endpoint source"));
		return;
	}
	UE::Tasks::Launch(UE_SOURCE_LOCATION, [this, Wallet, Payload, SuccessDelegate, ErrorDelegate]
	{
		FString Error;
		if (Thirdweb::ecosystem_wallet_link_account(ID, Wallet.GetID(), nullptr, nullptr, nullptr, TO_RUST_STRING(Payload)).AssignResult(Error, true))
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
	if (Source != Guest)
	{
		ErrorDelegate.Execute(TEXT("Wallet handle is not guest source"));
		return;
	}
	UE::Tasks::Launch(UE_SOURCE_LOCATION, [this, SuccessDelegate, ErrorDelegate]
	{
		FString Error;
		if (UThirdwebRuntimeSettings::IsEcosystem())
		{
			if (Thirdweb::ecosystem_wallet_sign_in_with_guest(ID, TO_RUST_STRING(FPlatformMisc::GetLoginId())).AssignResult(Error, true))
			{
				FThirdwebAnalytics::SendConnectEvent(ToAddress(), GetTypeString());
				SuccessDelegate.Execute();
				return;
			}
		}
		else
		{
			if (Thirdweb::in_app_wallet_sign_in_with_guest(ID, TO_RUST_STRING(FPlatformMisc::GetLoginId())).AssignResult(Error, true))
			{
				FThirdwebAnalytics::SendConnectEvent(ToAddress(), GetTypeString());
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
	if (Wallet.GetSource() != Guest)
	{
		ErrorDelegate.Execute(TEXT("Wallet handle is not guest source"));
		return;
	}
	UE::Tasks::Launch(UE_SOURCE_LOCATION, [this, Wallet, SuccessDelegate, ErrorDelegate]
	{
		FString Error;
		if (Thirdweb::ecosystem_wallet_link_account(ID, Wallet.GetID(), nullptr, nullptr, nullptr, TO_RUST_STRING(FPlatformMisc::GetLoginId())).AssignResult(Error, true))
		{
			SuccessDelegate.Execute();
			return;
		}
		ErrorDelegate.Execute(Error);
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
