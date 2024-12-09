// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Engine/Marketplace/AsyncTaskMarketplacePostBase.h"

void UAsyncTaskMarketplacePostBase::HandleResponse(const FString& QueueId)
{
	Success.Broadcast(QueueId, TEXT(""));
	SetReadyToDestroy();
}

void UAsyncTaskMarketplacePostBase::HandleFailed(const FString& Error)
{
	Failed.Broadcast(TEXT(""), Error);
	SetReadyToDestroy();
}
