// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "Wallets/ThirdwebSmartWalletHandle.h"

#include "Thirdweb.h"
#include "ThirdwebCommon.h"
#include "ThirdwebInternal.h"
#include "ThirdwebMacros.h"
#include "ThirdwebRuntimeSettings.h"
#include "ThirdwebSigner.h"
#include "ThirdwebUtils.h"

#include "Dom/JsonObject.h"

#include "GenericPlatform/GenericPlatformHttp.h"

#include "Kismet/KismetStringLibrary.h"

#include "Misc/DefaultValueHelper.h"

#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

FSmartWalletHandle::FSmartWalletHandle(const FInAppWalletHandle& InInAppWallet, const FString& Int64String)
{
	Type = Smart;
	int64 InID;
	FDefaultValueHelper::ParseInt64(Int64String, InID);
	ensureAlwaysMsgf(InID > 0, TEXT("Invalid id 0"));
	ID = InID;
}

FSmartWalletHandle FSmartWalletHandle::Create(const FInAppWalletHandle& InInAppWallet, const int64 ChainID, const bool bGasless, const FString& Factory, const FString& AccountOverride, bool& bSuccess, FString& Error)
{
	bSuccess = false;
	if (InInAppWallet.IsValid())
	{
		if (bSuccess = Thirdweb::create_smart_wallet(
				TO_RUST_STRING(UThirdwebRuntimeSettings::GetClientId()),
				TO_RUST_STRING(UThirdwebRuntimeSettings::GetBundleId()),
				nullptr,
				InInAppWallet.GetID(),
				TO_RUST_STRING(FString::Printf(TEXT("%lld"), ChainID)),
				bGasless,
				TO_RUST_STRING(Factory),
				TO_RUST_STRING(AccountOverride)
			).AssignResult(Error); bSuccess)
		{
			FSmartWalletHandle SmartWallet = FSmartWalletHandle(InInAppWallet, Error);
			FThirdwebAnalytics::SendConnectEvent(SmartWallet.ToAddress(), SmartWallet.GetTypeString());
			Error.Empty();
			return SmartWallet;
		}
	} else
	{
		Error = TEXT("Invalid InApp wallet");
	}
	
	return FSmartWalletHandle();
}

bool FSmartWalletHandle::IsDeployed(bool& bDeployed, FString& Error)
{
	if (!IsValid())
	{
		Error = TEXT("Invalid smart wallet");
		return false;
	}
	if (Thirdweb::smart_wallet_is_deployed(ID).AssignResult(Error))
	{
		bDeployed = Error.ToBool();
		Error.Empty();
		return true;
	}
	Error = TEXT("Not a networked wallet");
	return false;
}

bool FSmartWalletHandle::CreateSessionKey(const FString& Signer,
                                     const TArray<FString>& ApprovedTargets,
                                     const FString& NativeTokenLimitPerTransactionInWei,
                                     const FDateTime& PermissionStart,
                                     const FDateTime& PermissionEnd,
                                     const FDateTime& RequestValidityStart,
                                     const FDateTime& RequestValidityEnd,
                                     FString& TransactionHash,
                                     FString& Error)
{
	if (!IsValid())
	{
		Error = TEXT("Invalid smart wallet");
		return false;
	}
	FDateTime TenYearsFromNow = FDateTime::UtcNow() + FTimespan::FromDays(10 * 365);

	TArray<const char*> ApprovedTargetsCArray;
	for (const FString& Target : ApprovedTargets)
	{
		ApprovedTargetsCArray.Add(TO_RUST_STRING(Target));
	}
	if (Thirdweb::smart_wallet_create_session_key(
		ID,
		TO_RUST_STRING(Signer),
		ApprovedTargets.IsEmpty() ? nullptr : ApprovedTargetsCArray.GetData(),
		ApprovedTargetsCArray.Num(),
		TO_RUST_STRING(NativeTokenLimitPerTransactionInWei),
		TO_RUST_TIMESTAMP(PermissionStart),
		TO_RUST_TIMESTAMP(PermissionEnd),
		TO_RUST_TIMESTAMP(RequestValidityStart),
		TO_RUST_TIMESTAMP(RequestValidityEnd)
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

bool FSmartWalletHandle::GetAdmins(TArray<FString>& Admins, FString& Error)
{
	if (!IsValid())
	{
		Error = TEXT("Invalid smart wallet");
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

bool FSmartWalletHandle::GetActiveSigners(TArray<FSigner>& Signers, FString& Error)
{
	if (!IsValid())
	{
		Error = TEXT("Invalid smart wallet");
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

bool FSmartWalletHandle::RevokeSessionKey(const FString& Signer, FString& Error)
{
	if (!IsValid())
	{
		Error = TEXT("Invalid smart wallet");
		return false;
	}
	if (Thirdweb::smart_wallet_revoke_session_key(ID, TO_RUST_STRING(Signer)).AssignResult(Error))
	{
		Error.Empty();
		return true;
	}
	return false;;
}

bool FSmartWalletHandle::AddAdmin(const FString& Signer, FString& Error)
{
	if (!IsValid())
	{
		Error = TEXT("Invalid smart wallet");
		return false;
	}
	if (Thirdweb::smart_wallet_add_admin(ID, TO_RUST_STRING(Signer)).AssignResult(Error))
	{
		Error.Empty();
		return true;
	}
	return false;
}

bool FSmartWalletHandle::RemoveAdmin(const FString& Signer, FString& Error)
{
	if (!IsValid())
	{
		Error = TEXT("Invalid smart wallet");
		return false;
	}
	if (Thirdweb::smart_wallet_remove_admin(ID, TO_RUST_STRING(Signer)).AssignResult(Error))
	{
		Error.Empty();
		return true;
	}
	return false;
}