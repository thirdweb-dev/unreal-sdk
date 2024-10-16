// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Wallets/InApp/Create/AsyncTaskThirdwebInAppCreateWalletBase.h"

#include "Wallets/ThirdwebInAppWalletHandle.h"

void UAsyncTaskThirdwebInAppCreateWalletBase::HandleResponse(const FInAppWalletHandle& Wallet)
{
	if (IsInGameThread())
	{
		Success.Broadcast(Wallet, TEXT(""));
		SetReadyToDestroy();
	}
	else
	{
		// Retry on the GameThread.
		TWeakObjectPtr<UAsyncTaskThirdwebInAppCreateWalletBase> WeakThis = this;
		FFunctionGraphTask::CreateAndDispatchWhenReady([WeakThis, Wallet]()
		{
			if (WeakThis.IsValid())
			{
				WeakThis->HandleResponse(Wallet);
			}
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	}
}

void UAsyncTaskThirdwebInAppCreateWalletBase::HandleFailed(const FString& Error)
{
	if (IsInGameThread())
	{
		Failed.Broadcast(FInAppWalletHandle(), Error);
		SetReadyToDestroy();
	}
	else
	{
		// Retry on the GameThread.
		TWeakObjectPtr<UAsyncTaskThirdwebInAppCreateWalletBase> WeakThis = this;
		FFunctionGraphTask::CreateAndDispatchWhenReady([WeakThis, Error]()
		{
			if (WeakThis.IsValid())
			{
				WeakThis->HandleFailed(Error);
			}
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	}
}
