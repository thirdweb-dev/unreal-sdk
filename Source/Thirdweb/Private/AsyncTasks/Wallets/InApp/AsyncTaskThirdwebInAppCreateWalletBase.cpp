// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Wallets/InApp/AsyncTaskThirdwebInAppCreateWalletBase.h"

#include "Wallets/ThirdwebInAppWalletHandle.h"

void UAsyncTaskThirdwebInAppCreateWalletBase::HandleResponse(const FInAppWalletHandle& Wallet)
{
	Success.Broadcast(Wallet, TEXT(""));
	SetReadyToDestroy();
}

void UAsyncTaskThirdwebInAppCreateWalletBase::HandleFailed(const FString& Error)
{
	Failed.Broadcast(FInAppWalletHandle(), Error);
	SetReadyToDestroy();
}
