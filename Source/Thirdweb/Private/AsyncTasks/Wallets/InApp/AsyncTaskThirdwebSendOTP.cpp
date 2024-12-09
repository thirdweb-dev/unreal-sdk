// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Wallets/InApp/AsyncTaskThirdwebSendOTP.h"

#include "Async/TaskGraphInterfaces.h"
#include "Components/SlateWrapperTypes.h"

void UAsyncTaskThirdwebSendOTP::Activate()
{
	InAppWallet.SendOTP(BIND_UOBJECT_DELEGATE(FStreamableDelegate, HandleResponse), BIND_UOBJECT_DELEGATE(FStringDelegate, HandleFailed));
}

UAsyncTaskThirdwebSendOTP* UAsyncTaskThirdwebSendOTP::SendOTP(UObject* WorldContextObject, const FInAppWalletHandle& Wallet)
{
	if (!WorldContextObject)
	{
		return nullptr;
	}
	NEW_TASK
	Task->InAppWallet = Wallet;
	Task->RegisterWithGameInstance(WorldContextObject);
	return Task;
}

void UAsyncTaskThirdwebSendOTP::HandleResponse()
{
	if (IsInGameThread())
	{
		Success.Broadcast(TEXT(""));
		SetReadyToDestroy();
	}
	else
	{
		// Retry on the GameThread.
		TWeakObjectPtr<UAsyncTaskThirdwebSendOTP> WeakThis = this;
		FFunctionGraphTask::CreateAndDispatchWhenReady([WeakThis]()
		{
			if (WeakThis.IsValid())
			{
				WeakThis->HandleResponse();
			}
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	}
}

void UAsyncTaskThirdwebSendOTP::HandleFailed(const FString& Error)
{
	if (IsInGameThread())
	{
		Failed.Broadcast(Error);
		SetReadyToDestroy();
	}
	else
	{
		// Retry on the GameThread.
		TWeakObjectPtr<UAsyncTaskThirdwebSendOTP> WeakThis = this;
		FFunctionGraphTask::CreateAndDispatchWhenReady([WeakThis, Error]()
		{
			if (WeakThis.IsValid())
			{
				WeakThis->HandleFailed(Error);
			}
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	}
}
