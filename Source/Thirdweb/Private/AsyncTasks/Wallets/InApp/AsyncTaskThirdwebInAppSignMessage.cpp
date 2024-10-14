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
	Success.Broadcast(SignedMessage, TEXT(""));
	SetReadyToDestroy();
}

void UAsyncTaskThirdwebInAppSignMessage::HandleFailed(const FString& Error)
{
	Failed.Broadcast(TEXT(""), Error);
	SetReadyToDestroy();
}
