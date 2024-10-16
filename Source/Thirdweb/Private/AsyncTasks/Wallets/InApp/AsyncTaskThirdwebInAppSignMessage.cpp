// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Wallets/InApp/AsyncTaskThirdwebInAppSignMessage.h"

#include "Components/SlateWrapperTypes.h"

void UAsyncTaskThirdwebInAppSignMessage::Activate()
{
	InAppWallet.Sign(UnsignedMessage, BIND_UOBJECT_DELEGATE(FStringDelegate, HandleResponse), BIND_UOBJECT_DELEGATE(FStringDelegate, HandleFailed));
}

UAsyncTaskThirdwebInAppSignMessage* UAsyncTaskThirdwebInAppSignMessage::SignMessage(UObject* WorldContextObject, const FInAppWalletHandle& Wallet, const FString& Message)
{
	if (!WorldContextObject)
	{
		return nullptr;
	}
	NEW_TASK
	Task->InAppWallet = Wallet;
	Task->UnsignedMessage = Message;
	Task->RegisterWithGameInstance(WorldContextObject);
	return Task;
}

void UAsyncTaskThirdwebInAppSignMessage::HandleResponse(const FString& SignedMessage)
{
	if (IsInGameThread())
	{
		Success.Broadcast(SignedMessage, TEXT(""));
		SetReadyToDestroy();
	}
	else
	{
		// Retry on the GameThread.
		TWeakObjectPtr<UAsyncTaskThirdwebInAppSignMessage> WeakThis = this;
		FFunctionGraphTask::CreateAndDispatchWhenReady([WeakThis, SignedMessage]()
		{
			if (WeakThis.IsValid())
			{
				WeakThis->HandleResponse(SignedMessage);
			}
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	}
}

void UAsyncTaskThirdwebInAppSignMessage::HandleFailed(const FString& Error)
{
	if (IsInGameThread())
	{
		Failed.Broadcast(TEXT(""), Error);
		SetReadyToDestroy();
	}
	else
	{
		// Retry on the GameThread.
		TWeakObjectPtr<UAsyncTaskThirdwebInAppSignMessage> WeakThis = this;
		FFunctionGraphTask::CreateAndDispatchWhenReady([WeakThis, Error]()
		{
			if (WeakThis.IsValid())
			{
				WeakThis->HandleFailed(Error);
			}
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	}
}
