// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Wallets/InApp/Create/AsyncTaskThirdwebCreateSmartWallet.h"

#include "Components/SlateWrapperTypes.h"
#include "Wallets/ThirdwebSmartWalletHandle.h"

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
	if (!IsInGameThread())
	{
		TWeakObjectPtr<UAsyncTaskThirdwebCreateSmartWallet> WeakThis = this;
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

void UAsyncTaskThirdwebCreateSmartWallet::HandleFailed(const FString& Error)
{
	if (!IsInGameThread())
	{
		TWeakObjectPtr<UAsyncTaskThirdwebCreateSmartWallet> WeakThis = this;
		FFunctionGraphTask::CreateAndDispatchWhenReady([WeakThis, Error]()
		{
			if (WeakThis.IsValid())
			{
				WeakThis->HandleFailed(Error);
			}
		}, TStatId(), nullptr, ENamedThreads::GameThread);
		return;
	}
	
	Failed.Broadcast(FSmartWalletHandle(), Error);
	SetReadyToDestroy();
}
