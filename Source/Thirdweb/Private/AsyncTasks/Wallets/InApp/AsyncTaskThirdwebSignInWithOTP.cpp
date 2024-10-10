// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Wallets/InApp/AsyncTaskThirdwebSignInWithOTP.h"

void UAsyncTaskThirdwebSignInWithOTP::Activate()
{
	InAppWallet.SignInWithOTP(OTP, FStreamableDelegate::CreateUObject(this, &ThisClass::HandleResponse), FStringDelegate::CreateUObject(this, &ThisClass::HandleFailed));
}

UAsyncTaskThirdwebSignInWithOTP* UAsyncTaskThirdwebSignInWithOTP::SignInWithOTP(UObject* WorldContextObject, const FInAppWalletHandle& Wallet, const FString& OTP)
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

void UAsyncTaskThirdwebSignInWithOTP::HandleResponse()
{
	Success.Broadcast(TEXT(""));
	SetReadyToDestroy();
}

void UAsyncTaskThirdwebSignInWithOTP::HandleFailed(const FString& Error)
{
	Failed.Broadcast(Error);
	SetReadyToDestroy();
}
