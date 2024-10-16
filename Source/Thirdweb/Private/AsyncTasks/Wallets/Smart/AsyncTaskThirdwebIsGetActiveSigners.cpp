// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Wallets/Smart/AsyncTaskThirdwebGetActiveSigners.h"

#include "Components/SlateWrapperTypes.h"

#include "Containers/ThirdwebSigner.h"

void UAsyncTaskThirdwebGetActiveSigners::Activate()
{
	FString Error;
	SmartWallet.IsDeployed(BIND_UOBJECT_DELEGATE(FBoolDelegate, HandleIsDeployedResponse), BIND_UOBJECT_DELEGATE(FStringDelegate, HandleFailed));
}

UAsyncTaskThirdwebGetActiveSigners* UAsyncTaskThirdwebGetActiveSigners::GetActiveSigners(UObject* WorldContextObject, const FSmartWalletHandle& Wallet)
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

void UAsyncTaskThirdwebGetActiveSigners::HandleIsDeployedResponse(const bool& bDeployed)
{
	if (bDeployed)
	{
		return SmartWallet.GetActiveSigners(BIND_UOBJECT_DELEGATE(FSmartWalletHandle::FGetActiveSignersDelegate, HandleResponse), BIND_UOBJECT_DELEGATE(FStringDelegate, HandleFailed));
	}
	HandleResponse({});
}

void UAsyncTaskThirdwebGetActiveSigners::HandleResponse(const TArray<FSigner>& Signers)
{
	if (IsInGameThread())
	{
		Success.Broadcast(Signers, TEXT(""));
		SetReadyToDestroy();
	}
	else
	{
		// Retry on the GameThread.
		TWeakObjectPtr<UAsyncTaskThirdwebGetActiveSigners> WeakThis = this;
		FFunctionGraphTask::CreateAndDispatchWhenReady([WeakThis, Signers]()
		{
			if (WeakThis.IsValid())
			{
				WeakThis->HandleResponse(Signers);
			}
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	}
}

void UAsyncTaskThirdwebGetActiveSigners::HandleFailed(const FString& Error)
{
	if (IsInGameThread())
	{
		Failed.Broadcast({}, Error);
		SetReadyToDestroy();
	}
	else
	{
		// Retry on the GameThread.
		TWeakObjectPtr<UAsyncTaskThirdwebGetActiveSigners> WeakThis = this;
		FFunctionGraphTask::CreateAndDispatchWhenReady([WeakThis, Error]()
		{
			if (WeakThis.IsValid())
			{
				WeakThis->HandleFailed(Error);
			}
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	}
}
