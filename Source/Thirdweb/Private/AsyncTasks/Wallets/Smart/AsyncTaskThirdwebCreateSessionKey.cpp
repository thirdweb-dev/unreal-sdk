// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Wallets/Smart/AsyncTaskThirdwebCreateSessionKey.h"

#include "Components/SlateWrapperTypes.h"

void UAsyncTaskThirdwebCreateSessionKey::Activate()
{
	SmartWallet.CreateSessionKey(
		Signer,
		ApprovedTargets,
		NativeTokenLimitPerTransactionInWei,
		PermissionEnd,
		BIND_UOBJECT_DELEGATE(FStringDelegate, HandleResponse),
		BIND_UOBJECT_DELEGATE(FStringDelegate, HandleFailed)
	);
}

UAsyncTaskThirdwebCreateSessionKey* UAsyncTaskThirdwebCreateSessionKey::CreateSessionKey(UObject* WorldContextObject,
                                                                                         const FSmartWalletHandle& Wallet,
                                                                                         const FString& Signer,
                                                                                         const TArray<FString>& ApprovedTargets,
                                                                                         const FString& NativeTokenLimitPerTransactionInWei,
                                                                                         const FDateTime& PermissionEnd)
{
	if (!WorldContextObject)
	{
		return nullptr;
	}
	NEW_TASK

	Task->SmartWallet = Wallet;
	Task->Signer = Signer;
	Task->ApprovedTargets = ApprovedTargets;
	Task->NativeTokenLimitPerTransactionInWei = NativeTokenLimitPerTransactionInWei;
	Task->PermissionEnd = PermissionEnd;

	Task->RegisterWithGameInstance(WorldContextObject);
	return Task;
}

void UAsyncTaskThirdwebCreateSessionKey::HandleResponse(const FString& TxHash)
{
	if (!IsInGameThread())
	{
		TWeakObjectPtr<UAsyncTaskThirdwebCreateSessionKey> WeakThis = this;
		FFunctionGraphTask::CreateAndDispatchWhenReady([WeakThis, TxHash]()
		{
			if (WeakThis.IsValid())
			{
				WeakThis->HandleResponse(TxHash);
			}
		}, TStatId(), nullptr, ENamedThreads::GameThread);
		return;
	}
	
	Success.Broadcast(TxHash, TEXT(""));
	SetReadyToDestroy();
}

void UAsyncTaskThirdwebCreateSessionKey::HandleFailed(const FString& Error)
{
	if (!IsInGameThread())
	{
		TWeakObjectPtr<UAsyncTaskThirdwebCreateSessionKey> WeakThis = this;
		FFunctionGraphTask::CreateAndDispatchWhenReady([WeakThis, Error]()
		{
			if (WeakThis.IsValid())
			{
				WeakThis->HandleFailed(Error);
			}
		}, TStatId(), nullptr, ENamedThreads::GameThread);
		return;
	}
	Failed.Broadcast(TEXT(""), Error);
	SetReadyToDestroy();
}
