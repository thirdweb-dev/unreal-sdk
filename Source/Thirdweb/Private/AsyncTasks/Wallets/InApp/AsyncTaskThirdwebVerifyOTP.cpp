// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Wallets/InApp/AsyncTaskThirdwebVerifyOTP.h"

void UAsyncTaskThirdwebVerifyOTP::Activate()
{
	InAppWallet.SignInWithOTP(OTP, FStreamableDelegate::CreateUObject(this, &ThisClass::HandleResponse), FStringDelegate::CreateUObject(this, &ThisClass::HandleFailed));
}

UAsyncTaskThirdwebVerifyOTP* UAsyncTaskThirdwebVerifyOTP::VerifyOTP(UObject* WorldContextObject, const FInAppWalletHandle& Wallet, const FString& OTP)
{
	if (!WorldContextObject)
	{
		return nullptr;
	}
	NEW_TASK
	Task->InAppWallet = Wallet;
	Task->OTP = OTP;
	Task->RegisterWithGameInstance(WorldContextObject);
	return Task;
}

void UAsyncTaskThirdwebVerifyOTP::HandleResponse()
{
	Success.Broadcast(TEXT(""));
	SetReadyToDestroy();
}

void UAsyncTaskThirdwebVerifyOTP::HandleFailed(const FString& Error)
{
	Failed.Broadcast(Error);
	SetReadyToDestroy();
}
