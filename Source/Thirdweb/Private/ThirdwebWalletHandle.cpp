// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "ThirdwebWalletHandle.h"

#include "Misc/DefaultValueHelper.h"

FWalletHandle::FWalletHandle(const EWalletHandleType InType, const FString& InID)
{
	Type = InType;
	FDefaultValueHelper::ParseInt64(InID, ID);
}

FWalletHandle FWalletHandle::Invalid = FWalletHandle(InvalidHandle, 0);