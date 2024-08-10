// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "ThirdwebWalletHandle.h"

#include "Thirdweb.h"
#include "ThirdwebSigner.h"
#include "Kismet/KismetStringLibrary.h"
#include "Misc/DefaultValueHelper.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

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
	Error = TEXT("Wallet type must be InAppWallet for OTP action");
	return false;
}

bool FWalletHandle::SendOTP(FString& Error)
{
	if (Type == InApp)
	{
		return Thirdweb::in_app_wallet_send_otp(ID).AssignResult(Error, true);
	}
	Error = TEXT("Wallet type must be InAppWallet for OTP action");
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

bool FWalletHandle::CreateSessionKey(const FString& Signer, const TArray<FString>& ApprovedTargets, const FString& NativeTokenLimitPerTransactionInWei,
                                     const FDateTime& PermissionStart, const FDateTime& PermissionEnd, const FDateTime& RequestValidityStart, const FDateTime& RequestValidityEnd,
                                     FString& TransactionHash, FString& Error)
{
	if (Type != Smart)
	{
		Error = TEXT("Not a smart wallet");
		return false;
	}
	FDateTime TenYearsFromNow = FDateTime::UtcNow() + FTimespan::FromDays(10 * 365);
	
	TArray<const char*> ApprovedTargetsCArray;
	for (const FString& Target : ApprovedTargets)
	{
		ApprovedTargetsCArray.Add(Thirdweb::GetOrNull(Target));
	}
	if (Thirdweb::smart_wallet_create_session_key(
		ID,
		TCHAR_TO_UTF8(*Signer),
		ApprovedTargets.IsEmpty() ? nullptr : ApprovedTargetsCArray.GetData(),
		ApprovedTargetsCArray.Num(),
		TCHAR_TO_UTF8(*NativeTokenLimitPerTransactionInWei),
		PermissionStart == FDateTime::MinValue() ? 0 : PermissionStart.ToUnixTimestamp(),
		PermissionEnd == FDateTime::MinValue() ? TenYearsFromNow.ToUnixTimestamp() : PermissionEnd.ToUnixTimestamp(),
		RequestValidityStart == FDateTime::MinValue() ? 0 : RequestValidityStart.ToUnixTimestamp(),
		RequestValidityEnd == FDateTime::MinValue() ? TenYearsFromNow.ToUnixTimestamp() : RequestValidityEnd.ToUnixTimestamp()
	).AssignResult(Error))
	{
		TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
		const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Error);
		if (FJsonSerializer::Deserialize(Reader, JsonObject); JsonObject.IsValid())
		{
			if (JsonObject->HasTypedField<EJson::String>(TEXT("transactionHash")))
			{
				TransactionHash = JsonObject->GetStringField(TEXT("transactionHash"));
			}
		}
		Error.Empty();
		return true;
	}
	return false;
}

bool FWalletHandle::GetAdmins(TArray<FString>& Admins, FString& Error)
{
	if (Type != Smart)
	{
		Error = TEXT("Not a smart wallet");
		return false;
	}
	if (Thirdweb::smart_wallet_get_all_admins(ID).AssignResult(Error))
	{
		TArray<TSharedPtr<FJsonValue>> JsonValueArray;
		const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Error);
		FJsonSerializer::Deserialize(Reader, JsonValueArray);
		for (int i = 0; i < JsonValueArray.Num(); i++)
		{
			if (JsonValueArray[i]->Type == EJson::String)
			{
				Admins.Emplace(JsonValueArray[i]->AsString());
			}
		}
		Error.Empty();
		return true;
	}
	return false;
}

bool FWalletHandle::GetActiveSigners(TArray<FSigner>& Signers, FString& Error)
{
	if (Type != Smart)
	{
		Error = TEXT("Not a smart wallet");
		return false;
	}
	if (Thirdweb::smart_wallet_get_all_active_signers(ID).AssignResult(Error))
	{
		TArray<TSharedPtr<FJsonValue>> JsonValueArray;
		const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Error);
		FJsonSerializer::Deserialize(Reader, JsonValueArray);
		for (int i = 0; i < JsonValueArray.Num(); i++)
		{
			if (JsonValueArray[i]->Type == EJson::Object)
			{
				Signers.Emplace(FSigner::FromJson(JsonValueArray[i]->AsObject()));
			}
		}
		Error.Empty();
		return true;
	}
	return false;
}

bool FWalletHandle::RevokeSessionKey(const FString& Signer, FString& Error)
{
	if (Type != Smart)
	{
		Error = TEXT("Not a smart wallet");
		return false;
	}
	if (Thirdweb::smart_wallet_revoke_session_key(ID, Signer.IsEmpty() ? nullptr : TCHAR_TO_UTF8(*Signer)).AssignResult(Error))
	{
		Error.Empty();
		return true;
	}
	return false;;
}

bool FWalletHandle::AddAdmin(const FString& Signer, FString& Error)
{
	if (Type != Smart)
	{
		Error = TEXT("Not a smart wallet");
		return false;
	}
	if (Thirdweb::smart_wallet_add_admin(ID, Signer.IsEmpty() ? nullptr : TCHAR_TO_UTF8(*Signer)).AssignResult(Error))
	{
		Error.Empty();
		return true;
	}
	return false;
}

bool FWalletHandle::RemoveAdmin(const FString& Signer, FString& Error)
{
	if (Type != Smart)
	{
		Error = TEXT("Not a smart wallet");
		return false;
	}
	if (Thirdweb::smart_wallet_remove_admin(ID, Signer.IsEmpty() ? nullptr : TCHAR_TO_UTF8(*Signer)).AssignResult(Error))
	{
		Error.Empty();
		return true;
	}
	return false;
}

FString FWalletHandle::Sign(const FString& Message) const { return Thirdweb::sign_message(ID, StringCast<ANSICHAR>(*Message).Get()).GetOutput(); }
