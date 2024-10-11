// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Wallets/InApp/AsyncTaskThirdwebSignInWithOTP.h"

#include "Components/SlateWrapperTypes.h"

void UAsyncTaskThirdwebSignInWithOTP::Activate()
{
	InAppWallet.SignInWithOTP(OTP, BIND_UOBJECT_DELEGATE(FStreamableDelegate, HandleResponse), BIND_UOBJECT_DELEGATE(FStringDelegate, HandleFailed));
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
