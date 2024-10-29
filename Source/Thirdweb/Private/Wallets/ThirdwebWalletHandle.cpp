// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "Wallets/ThirdwebWalletHandle.h"

#include "Thirdweb.h"
#include "ThirdwebCommon.h"
#include "ThirdwebLog.h"
#include "ThirdwebMacros.h"
#include "ThirdwebUtils.h"

#include "Tasks/Task.h"

#include "Wallets/ThirdwebSmartWalletHandle.h"

FWalletHandle::FWalletHandle(const FInAppWalletHandle& InAppWalletHandle)
{
	Type = InAppWalletHandle.GetType();
	ID = InAppWalletHandle.GetID();
}

FWalletHandle::FWalletHandle(const FSmartWalletHandle& SmartWalletHandle)
{
	Type = SmartWalletHandle.GetType();
	ID = SmartWalletHandle.GetID();
}

FString FWalletHandle::ToAddress() const
{
	if (!CachedAddress.IsEmpty())
	{
		return CachedAddress;
	}
	FString Result;
	return Thirdweb::get_wallet_address(ID).AssignResult(Result) ? Result : ThirdwebUtils::ZeroAddress;
}

void FWalletHandle::CacheAddress()
{
	if (!CachedAddress.IsEmpty())
	{
		if (FString Result; Thirdweb::get_wallet_address(ID).AssignResult(Result))
		{
			CachedAddress = Result;
		}
	}
}

void FWalletHandle::Sign(const FString& Message, const FStringDelegate& SuccessDelegate, const FStringDelegate& ErrorDelegate) const
{
	CHECK_DELEGATES(SuccessDelegate, ErrorDelegate)
	CHECK_VALIDITY(ErrorDelegate)

	FWalletHandle ThisCopy = *this;
	UE::Tasks::Launch(UE_SOURCE_LOCATION, [ThisCopy, Message, SuccessDelegate, ErrorDelegate]
	{
		if (FString Error; Thirdweb::sign_message(ThisCopy.GetID(), TO_RUST_STRING(Message)).AssignResult(Error))
		{
			SuccessDelegate.Execute(Error);
		} else
		{
			ErrorDelegate.Execute(Error);
		}
	});
}
