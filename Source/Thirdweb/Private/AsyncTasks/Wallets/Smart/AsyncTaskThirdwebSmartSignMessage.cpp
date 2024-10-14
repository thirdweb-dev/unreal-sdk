// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Wallets/Smart/AsyncTaskThirdwebSmartSignMessage.h"

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
	Success.Broadcast(SignedMessage, TEXT(""));
	SetReadyToDestroy();
}

void UAsyncTaskThirdwebSmartSignMessage::HandleFailed(const FString& Error)
{
	Failed.Broadcast(TEXT(""), Error);
	SetReadyToDestroy();
}
