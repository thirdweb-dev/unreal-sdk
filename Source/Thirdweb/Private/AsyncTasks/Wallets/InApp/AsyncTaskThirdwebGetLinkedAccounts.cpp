// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Wallets/InApp/AsyncTaskThirdwebGetLinkedAccounts.h"

#include "Components/SlateWrapperTypes.h"

void UAsyncTaskThirdwebGetLinkedAccounts::Activate()
{
	InAppWallet.GetLinkedAccounts(BIND_UOBJECT_DELEGATE(FInAppWalletHandle::FGetLinkedAccountsDelegate, HandleResponse), BIND_UOBJECT_DELEGATE(FStringDelegate, HandleFailed));
}

UAsyncTaskThirdwebGetLinkedAccounts* UAsyncTaskThirdwebGetLinkedAccounts::GetLinkedAccounts(UObject* WorldContextObject, const FInAppWalletHandle& Wallet)
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

void UAsyncTaskThirdwebGetLinkedAccounts::HandleResponse(const TArray<FString>& LinkedAccounts)
{
	if (IsInGameThread())
	{
		Success.Broadcast(LinkedAccounts, TEXT(""));
		SetReadyToDestroy();
	}
	else
	{
		// Retry on the GameThread.
		TWeakObjectPtr<UAsyncTaskThirdwebGetLinkedAccounts> WeakThis = this;
		FFunctionGraphTask::CreateAndDispatchWhenReady([WeakThis, LinkedAccounts]()
		{
			if (WeakThis.IsValid())
			{
				WeakThis->HandleResponse(LinkedAccounts);
			}
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	}
}

void UAsyncTaskThirdwebGetLinkedAccounts::HandleFailed(const FString& Error)
{
	if (IsInGameThread())
	{
		Failed.Broadcast({}, Error);
		SetReadyToDestroy();
	}
	else
	{
		// Retry on the GameThread.
		TWeakObjectPtr<UAsyncTaskThirdwebGetLinkedAccounts> WeakThis = this;
		FFunctionGraphTask::CreateAndDispatchWhenReady([WeakThis, Error]()
		{
			if (WeakThis.IsValid())
			{
				WeakThis->HandleFailed(Error);
			}
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	}
}
