// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Wallets/InApp/SignIn/AsyncTaskThirdwebSignInBase.h"

void UAsyncTaskThirdwebSignInBase::HandleResponse()
{
	if (IsInGameThread())
	{
		Success.Broadcast(TEXT(""));
		SetReadyToDestroy();
	}
	else
	{
		// Retry on the GameThread.
		TWeakObjectPtr<UAsyncTaskThirdwebSignInBase> WeakThis = this;
		FFunctionGraphTask::CreateAndDispatchWhenReady([WeakThis]()
		{
			if (WeakThis.IsValid())
			{
				WeakThis->HandleResponse();
			}
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	}
}

void UAsyncTaskThirdwebSignInBase::HandleFailed(const FString& Error)
{
	if (IsInGameThread())
	{
		Failed.Broadcast(Error);
		SetReadyToDestroy();
	}
	else
	{
		// Retry on the GameThread.
		TWeakObjectPtr<UAsyncTaskThirdwebSignInBase> WeakThis = this;
		FFunctionGraphTask::CreateAndDispatchWhenReady([WeakThis, Error]()
		{
			if (WeakThis.IsValid())
			{
				WeakThis->HandleFailed(Error);
			}
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	}
}
