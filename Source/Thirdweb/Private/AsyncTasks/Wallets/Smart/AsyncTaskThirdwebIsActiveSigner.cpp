// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Wallets/Smart/AsyncTaskThirdwebIsActiveSigner.h"

#include "Containers/ThirdwebSigner.h"

#include "Components/SlateWrapperTypes.h"

void UAsyncTaskThirdwebIsActiveSigner::Activate()
{
	FString Error;
	SmartWallet.IsDeployed(BIND_UOBJECT_DELEGATE(FBoolDelegate, HandleIsDeployedResponse), BIND_UOBJECT_DELEGATE(FStringDelegate, HandleFailed));
}

UAsyncTaskThirdwebIsActiveSigner* UAsyncTaskThirdwebIsActiveSigner::IsActiveSigner(UObject* WorldContextObject, const FSmartWalletHandle& Wallet, const FString& BackendWallet)
{
	if (!WorldContextObject)
	{
		return nullptr;
	}
	NEW_TASK
	Task->SmartWallet = Wallet;
	Task->BackendWallet = BackendWallet;
	Task->RegisterWithGameInstance(WorldContextObject);
	return Task;
}

void UAsyncTaskThirdwebIsActiveSigner::HandleIsDeployedResponse(const bool bDeployed)
{
	if (bDeployed)
	{
		return SmartWallet.GetActiveSigners(BIND_UOBJECT_DELEGATE(FSmartWalletHandle::FGetActiveSignersDelegate, HandleResponse), BIND_UOBJECT_DELEGATE(FStringDelegate, HandleFailed));
	}
	HandleResponse({});
}

void UAsyncTaskThirdwebIsActiveSigner::HandleResponse(const TArray<FSigner>& Signers)
{
	if (IsInGameThread())
	{
		for (int i = 0; i < Signers.Num(); i++)
		{
			if (Signers[i].Address.Equals(BackendWallet, ESearchCase::IgnoreCase))
			{
				Success.Broadcast(true, TEXT(""));
				return SetReadyToDestroy();
			}
		}
	}
	else
	{
		// Retry on the GameThread.
		TWeakObjectPtr<UAsyncTaskThirdwebIsActiveSigner> WeakThis = this;
		FFunctionGraphTask::CreateAndDispatchWhenReady([WeakThis, Signers]()
		{
			if (WeakThis.IsValid())
			{
				WeakThis->HandleResponse(Signers);
			}
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	}
}

void UAsyncTaskThirdwebIsActiveSigner::HandleFailed(const FString& Error)
{
	if (IsInGameThread())
	{
		Failed.Broadcast(false, Error);
		SetReadyToDestroy();
	}
	else
	{
		// Retry on the GameThread.
		TWeakObjectPtr<UAsyncTaskThirdwebIsActiveSigner> WeakThis = this;
		FFunctionGraphTask::CreateAndDispatchWhenReady([WeakThis, Error]()
		{
			if (WeakThis.IsValid())
			{
				WeakThis->HandleFailed(Error);
			}
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	}
}
