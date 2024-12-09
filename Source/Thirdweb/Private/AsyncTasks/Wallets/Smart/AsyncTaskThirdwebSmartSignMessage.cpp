// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Wallets/Smart/AsyncTaskThirdwebSmartSignMessage.h"

#include "Async/TaskGraphInterfaces.h"
#include "Components/SlateWrapperTypes.h"

void UAsyncTaskThirdwebSmartSignMessage::Activate()
{
	SmartWallet.Sign(UnsignedMessage, BIND_UOBJECT_DELEGATE(FStringDelegate, HandleResponse), BIND_UOBJECT_DELEGATE(FStringDelegate, HandleFailed));
}

UAsyncTaskThirdwebSmartSignMessage* UAsyncTaskThirdwebSmartSignMessage::SignMessage(UObject* WorldContextObject, const FSmartWalletHandle& Wallet, const FString& Message)
{
	if (!WorldContextObject)
	{
		return nullptr;
	}
	NEW_TASK
	Task->SmartWallet = Wallet;
	Task->UnsignedMessage = Message;
	Task->RegisterWithGameInstance(WorldContextObject);
	return Task;
}

void UAsyncTaskThirdwebSmartSignMessage::HandleResponse(const FString& SignedMessage)
{
	if (IsInGameThread())
	{
		Success.Broadcast(SignedMessage, TEXT(""));
		SetReadyToDestroy();
	}
	else
	{
		// Retry on the GameThread.
		TWeakObjectPtr<UAsyncTaskThirdwebSmartSignMessage> WeakThis = this;
		FFunctionGraphTask::CreateAndDispatchWhenReady([WeakThis, SignedMessage]()
		{
			if (WeakThis.IsValid())
			{
				WeakThis->HandleResponse(SignedMessage);
			}
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	}
}

void UAsyncTaskThirdwebSmartSignMessage::HandleFailed(const FString& Error)
{
	if (IsInGameThread())
	{
		Failed.Broadcast(TEXT(""), Error);
		SetReadyToDestroy();
	}
	else
	{
		// Retry on the GameThread.
		TWeakObjectPtr<UAsyncTaskThirdwebSmartSignMessage> WeakThis = this;
		FFunctionGraphTask::CreateAndDispatchWhenReady([WeakThis, Error]()
		{
			if (WeakThis.IsValid())
			{
				WeakThis->HandleFailed(Error);
			}
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	}
}
