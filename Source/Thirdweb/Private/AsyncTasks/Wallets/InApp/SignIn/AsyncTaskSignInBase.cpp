// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Wallets/InApp/SignIn/AsyncTaskThirdwebSignInBase.h"

void UAsyncTaskThirdwebSignInBase::HandleResponse()
{
	Success.Broadcast(TEXT(""));
	SetReadyToDestroy();
}

void UAsyncTaskThirdwebSignInBase::HandleFailed(const FString& Error)
{
	Failed.Broadcast(Error);
	SetReadyToDestroy();
}
