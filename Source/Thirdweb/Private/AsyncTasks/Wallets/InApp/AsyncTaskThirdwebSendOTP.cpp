// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Wallets/InApp/AsyncTaskThirdwebSendOTP.h"

void UAsyncTaskThirdwebSendOTP::Activate()
{
	InAppWallet.SendOTP(FStreamableDelegate::CreateUObject(this, &ThisClass::HandleResponse), FStringDelegate::CreateUObject(this, &ThisClass::HandleFailed));
}

UAsyncTaskThirdwebSendOTP* UAsyncTaskThirdwebSendOTP::SendOTP(UObject* WorldContextObject, const FInAppWalletHandle& Wallet)
{
	if (!WorldContextObject)
	{
		return nullptr;
	}
	NEW_TASK
	Task->InAppWallet = Wallet;
	Task->RegisterWithGameInstance(WorldContextObject);
	return Task;
}

void UAsyncTaskThirdwebSendOTP::HandleResponse()
{
	Success.Broadcast(TEXT(""));
	SetReadyToDestroy();
}

void UAsyncTaskThirdwebSendOTP::HandleFailed(const FString& Error)
{
	Failed.Broadcast(Error);
	SetReadyToDestroy();
}
