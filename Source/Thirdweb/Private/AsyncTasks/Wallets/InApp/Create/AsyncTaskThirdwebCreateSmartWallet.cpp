// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Wallets/InApp/Create/AsyncTaskThirdwebCreateSmartWallet.h"

#include "Components/SlateWrapperTypes.h"

#include "Wallets/ThirdwebSmartWalletHandle.h"

UAsyncTaskThirdwebCreateSmartWallet* UAsyncTaskThirdwebCreateSmartWallet::CreateSmartWallet(UObject* WorldContextObject,
                                                                                            const FInAppWalletHandle& InAppWallet,
                                                                                            const int64 ChainID,
                                                                                            const bool bGasless,
                                                                                            const FString& Factory,
                                                                                            const FString& AccountOverride)
{
	if (!WorldContextObject)
	{
		return nullptr;
	}
	NEW_TASK

	Task->InAppWallet = InAppWallet;
	Task->ChainID = ChainID;
	Task->bGasless = bGasless;
	Task->Factory = Factory;
	Task->AccountOverride = AccountOverride;

	Task->RegisterWithGameInstance(WorldContextObject);
	return Task;
}

void UAsyncTaskThirdwebCreateSmartWallet::Activate()
{
	FSmartWalletHandle::Create(
		InAppWallet,
		ChainID,
		bGasless,
		Factory,
		AccountOverride,
		BIND_UOBJECT_DELEGATE(FSmartWalletHandle::FCreateSmartWalletDelegate, HandleResponse),
		BIND_UOBJECT_DELEGATE(FStringDelegate, HandleFailed)
	);
}

void UAsyncTaskThirdwebCreateSmartWallet::HandleResponse(const FSmartWalletHandle& Wallet)
{
	if (IsInGameThread())
	{
		Success.Broadcast(Wallet, TEXT(""));
		SetReadyToDestroy();
	}
	else
	{
		// Retry on the GameThread.
		TWeakObjectPtr<UAsyncTaskThirdwebCreateSmartWallet> WeakThis = this;
		FFunctionGraphTask::CreateAndDispatchWhenReady([WeakThis, Wallet]()
		{
			if (WeakThis.IsValid())
			{
				WeakThis->HandleResponse(Wallet);
			}
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	}
}

void UAsyncTaskThirdwebCreateSmartWallet::HandleFailed(const FString& Error)
{
	if (IsInGameThread())
	{
		Failed.Broadcast(FSmartWalletHandle(), Error);
		SetReadyToDestroy();
	}
	else
	{
		// Retry on the GameThread.
		TWeakObjectPtr<UAsyncTaskThirdwebCreateSmartWallet> WeakThis = this;
		FFunctionGraphTask::CreateAndDispatchWhenReady([WeakThis, Error]()
		{
			if (WeakThis.IsValid())
			{
				WeakThis->HandleFailed(Error);
			}
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	}
}
