// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Wallets/Smart/AsyncTaskThirdwebRemoveAdmin.h"

#include "ThirdwebSigner.h"

#include "Components/SlateWrapperTypes.h"

void UAsyncTaskThirdwebRemoveAdmin::Activate()
{
	FString Error;
	SmartWallet.RemoveAdmin(Signer, BIND_UOBJECT_DELEGATE(FSimpleDelegate, HandleResponse), BIND_UOBJECT_DELEGATE(FStringDelegate, HandleFailed));
}

UAsyncTaskThirdwebRemoveAdmin* UAsyncTaskThirdwebRemoveAdmin::RemoveAdmin(UObject* WorldContextObject, const FSmartWalletHandle& Wallet, const FString& Signer)
{
	if (!WorldContextObject)
	{
		return nullptr;
	}
	NEW_TASK
	Task->SmartWallet = Wallet;
	Task->Signer = Signer;
	Task->RegisterWithGameInstance(WorldContextObject);
	return Task;
}

void UAsyncTaskThirdwebRemoveAdmin::HandleResponse()
{
	Success.Broadcast(TEXT(""));
	return SetReadyToDestroy();
}

void UAsyncTaskThirdwebRemoveAdmin::HandleFailed(const FString& Error)
{
	Failed.Broadcast(Error);
	SetReadyToDestroy();
}
