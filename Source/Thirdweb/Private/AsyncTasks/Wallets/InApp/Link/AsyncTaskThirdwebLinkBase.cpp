// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Wallets/InApp/Link/AsyncTaskThirdwebLinkBase.h"

void UAsyncTaskThirdwebLinkBase::HandleResponse()
{
	if (IsInGameThread())
	{
		Success.Broadcast(TEXT(""));
		SetReadyToDestroy();
	}
	else
	{
		// Retry on the GameThread.
		TWeakObjectPtr<UAsyncTaskThirdwebLinkBase> WeakThis = this;
		FFunctionGraphTask::CreateAndDispatchWhenReady([WeakThis]()
		{
			if (WeakThis.IsValid())
			{
				WeakThis->HandleResponse();
			}
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	}
}

void UAsyncTaskThirdwebLinkBase::HandleFailed(const FString& Error)
{
	if (IsInGameThread())
	{
		Failed.Broadcast(Error);
		SetReadyToDestroy();
	}
	else
	{
		// Retry on the GameThread.
		TWeakObjectPtr<UAsyncTaskThirdwebLinkBase> WeakThis = this;
		FFunctionGraphTask::CreateAndDispatchWhenReady([WeakThis, Error]()
		{
			if (WeakThis.IsValid())
			{
				WeakThis->HandleFailed(Error);
			}
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	}
}
