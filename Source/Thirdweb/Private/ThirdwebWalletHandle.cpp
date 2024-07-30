// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "ThirdwebWalletHandle.h"

#include "Thirdweb.h"
#include "Misc/DefaultValueHelper.h"

FWalletHandle::FWalletHandle(const EWalletHandleType InType, const FString& Int64String)
{
	ensureAlwaysMsgf(InType != InvalidHandle, TEXT("Invalid handle type"));
	Type = InType;
	int64 InID;
	FDefaultValueHelper::ParseInt64(Int64String, InID);
	ensureAlwaysMsgf(InID > 0, TEXT("Invalid id 0"));
	ID = InID;
}

FWalletHandle FWalletHandle::FromPrivateKey(const FString& PrivateKey)
{
	int64 ID;
	FDefaultValueHelper::ParseInt64(Thirdweb::create_private_key_wallet(StringCast<ANSICHAR>(*PrivateKey).Get()).GetOutput(), ID);
	return FWalletHandle(EWalletHandleType::PrivateKey, ID);
}

FWalletHandle FWalletHandle::GeneratePrivateKey()
{
	int64 ID;
	FDefaultValueHelper::ParseInt64(Thirdweb::generate_private_key_wallet().GetOutput(), ID);
	return FWalletHandle(PrivateKey, ID);
}

bool FWalletHandle::IsDeployed(bool& bDeployed, FString& Error)
{
	if (Type == Smart)
	{
		if (Thirdweb::smart_wallet_is_deployed(ID).AssignResult(Error))
		{
			bDeployed = Error.ToBool();
			Error.Empty();
			return true;
		}
	}
	Error = TEXT("Not a networked wallet");
	return false;
}

bool FWalletHandle::IsConnected() const
{
	return Thirdweb::is_connected(ID).GetOutput().ToBool();
}

void FWalletHandle::Disconnect() const
{
	Thirdweb::disconnect(ID).Free();
}

FString FWalletHandle::ToAddress() const { return Thirdweb::get_wallet_address(ID).GetOutput(); }

bool FWalletHandle::VerifyOTP(const FString& OTP, bool& CanRetry, FString& Error)
{
	if (Type == InApp)
	{
		return Thirdweb::in_app_wallet_verify_otp(ID, StringCast<ANSICHAR>(*OTP).Get()).AssignRetryResult(CanRetry, Error, true);
	}
	Error = TEXT("Wallet type must be InAppWallet for OAuth action");
	return false;
}

bool FWalletHandle::SendOTP(FString& Error)
{
	
	if (Type == InApp)
	{
		return Thirdweb::in_app_wallet_send_otp(ID).AssignResult(Error, true);
	}
	Error = TEXT("Wallet type must be InAppWallet for OAuth action");
	return false;
}

bool FWalletHandle::FetchOAuthLoginURL(const FString& RedirectUrl, FString& LoginLink, FString& Error)
{
	if (Type == InApp)
	{
		if (Thirdweb::in_app_wallet_fetch_oauth_login_link(ID, StringCast<ANSICHAR>(*RedirectUrl).Get()).AssignResult(Error))
		{
			LoginLink = Error;
			Error.Empty();
			return true;
		}
	}
	Error = TEXT("Wallet type must be InAppWallet for OAuth action");
	return false;
}

bool FWalletHandle::SignInWithOAuth(const FString& AuthResult, FString& Error)
{

	if (Type == InApp)
	{
		return Thirdweb::in_app_wallet_sign_in_with_oauth(ID, StringCast<ANSICHAR>(*AuthResult).Get()).AssignResult(Error);
	}
	Error = TEXT("Wallet type must be InAppWallet for OAuth action");
	return false;
}

FString FWalletHandle::Sign(const FString& Message) const { return Thirdweb::sign_message(ID, StringCast<ANSICHAR>(*Message).Get()).GetOutput(); }

