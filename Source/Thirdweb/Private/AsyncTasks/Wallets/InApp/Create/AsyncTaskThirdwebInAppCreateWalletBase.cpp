// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Wallets/InApp/Create/AsyncTaskThirdwebInAppCreateWalletBase.h"

#include "Async/TaskGraphInterfaces.h"
#include "Wallets/ThirdwebInAppWalletHandle.h"

void UAsyncTaskThirdwebInAppCreateWalletBase::HandleResponse(const FInAppWalletHandle& Wallet)
{
	if (!IsInGameThread())
	{
		TWeakObjectPtr<UAsyncTaskThirdwebInAppCreateWalletBase> WeakThis = this;
		FFunctionGraphTask::CreateAndDispatchWhenReady([WeakThis, Wallet]()
		{
			if (WeakThis.IsValid())
			{
				WeakThis->HandleResponse(Wallet);
			}
		}, TStatId(), nullptr, ENamedThreads::GameThread);
		return;
	}

	Success.Broadcast(Wallet, TEXT(""));
	SetReadyToDestroy();
}

void UAsyncTaskThirdwebInAppCreateWalletBase::HandleFailed(const FString& Error)
{
	if (!IsInGameThread())
	{
		TWeakObjectPtr<UAsyncTaskThirdwebInAppCreateWalletBase> WeakThis = this;
		FFunctionGraphTask::CreateAndDispatchWhenReady([WeakThis, Error]()
		{
			if (WeakThis.IsValid())
			{
				WeakThis->HandleFailed(Error);
			}
		}, TStatId(), nullptr, ENamedThreads::GameThread);
		return;
	}

	Failed.Broadcast(FInAppWalletHandle(), Error);
	SetReadyToDestroy();
}
