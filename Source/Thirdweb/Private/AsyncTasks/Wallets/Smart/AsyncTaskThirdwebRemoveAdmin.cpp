// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Wallets/Smart/AsyncTaskThirdwebRemoveAdmin.h"

#include "Async/TaskGraphInterfaces.h"
#include "Components/SlateWrapperTypes.h"

void UAsyncTaskThirdwebRemoveAdmin::Activate()
{
	FString Error;
	SmartWallet.RemoveAdmin(Signer, BIND_UOBJECT_DELEGATE(FSimpleDelegate, HandleResponse), BIND_UOBJECT_DELEGATE(FStringDelegate, HandleFailed));
}

UAsyncTaskThirdwebRemoveAdmin* UAsyncTaskThirdwebRemoveAdmin::RemoveAdmin(UObject* WorldContextObject, const FSmartWalletHandle& Wallet, const FString& Signer)
{
	if (!WorldContextObject)
	{
		return nullptr;
	}
	NEW_TASK
	Task->SmartWallet = Wallet;
	Task->Signer = Signer;
	Task->RegisterWithGameInstance(WorldContextObject);
	return Task;
}

void UAsyncTaskThirdwebRemoveAdmin::HandleResponse()
{
	if (IsInGameThread())
	{
		Success.Broadcast(TEXT(""));
		return SetReadyToDestroy();
	}
	// Retry on the GameThread.
	TWeakObjectPtr<UAsyncTaskThirdwebRemoveAdmin> WeakThis = this;
	FFunctionGraphTask::CreateAndDispatchWhenReady([WeakThis]()
	{
		if (WeakThis.IsValid())
		{
			WeakThis->HandleResponse();
		}
	}, TStatId(), nullptr, ENamedThreads::GameThread);
}

void UAsyncTaskThirdwebRemoveAdmin::HandleFailed(const FString& Error)
{
	if (IsInGameThread())
	{
		Failed.Broadcast(Error);
		SetReadyToDestroy();
	}
	else
	{
		// Retry on the GameThread.
		TWeakObjectPtr<UAsyncTaskThirdwebRemoveAdmin> WeakThis = this;
		FFunctionGraphTask::CreateAndDispatchWhenReady([WeakThis, Error]()
		{
			if (WeakThis.IsValid())
			{
				WeakThis->HandleFailed(Error);
			}
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	}
}
