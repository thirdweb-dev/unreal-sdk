// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Wallets/Smart/AsyncTaskThirdwebGetAdmins.h"

#include "Async/TaskGraphInterfaces.h"
#include "Components/SlateWrapperTypes.h"

void UAsyncTaskThirdwebGetAdmins::Activate()
{
	SmartWallet.GetAdmins(BIND_UOBJECT_DELEGATE(FStringArrayDelegate, HandleResponse), BIND_UOBJECT_DELEGATE(FStringDelegate, HandleFailed));
}

UAsyncTaskThirdwebGetAdmins* UAsyncTaskThirdwebGetAdmins::GetAdmins(UObject* WorldContextObject, const FSmartWalletHandle& Wallet)
{
	if (!WorldContextObject)
	{
		return nullptr;
	}
	NEW_TASK
	Task->SmartWallet = Wallet;
	Task->RegisterWithGameInstance(WorldContextObject);
	return Task;
}

void UAsyncTaskThirdwebGetAdmins::HandleResponse(const TArray<FString>& Admins)
{
	if (IsInGameThread())
	{
		Success.Broadcast(Admins, TEXT(""));
		SetReadyToDestroy();
	}
	else
	{
		// Retry on the GameThread.
		TWeakObjectPtr<UAsyncTaskThirdwebGetAdmins> WeakThis = this;
		FFunctionGraphTask::CreateAndDispatchWhenReady([WeakThis, Admins]()
		{
			if (WeakThis.IsValid())
			{
				WeakThis->HandleResponse(Admins);
			}
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	}
}

void UAsyncTaskThirdwebGetAdmins::HandleFailed(const FString& Error)
{
	if (IsInGameThread())
	{
		Failed.Broadcast({}, Error);
		SetReadyToDestroy();
	}
	else
	{
		// Retry on the GameThread.
		TWeakObjectPtr<UAsyncTaskThirdwebGetAdmins> WeakThis = this;
		FFunctionGraphTask::CreateAndDispatchWhenReady([WeakThis, Error]()
		{
			if (WeakThis.IsValid())
			{
				WeakThis->HandleFailed(Error);
			}
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	}
}
