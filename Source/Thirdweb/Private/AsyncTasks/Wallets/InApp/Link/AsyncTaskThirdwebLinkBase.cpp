// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Wallets/InApp/Link/AsyncTaskThirdwebLinkBase.h"

void UAsyncTaskThirdwebLinkBase::HandleResponse()
{
	Success.Broadcast(TEXT(""));
	SetReadyToDestroy();
}

void UAsyncTaskThirdwebLinkBase::HandleFailed(const FString& Error)
{
	Failed.Broadcast(Error);
	SetReadyToDestroy();
}
