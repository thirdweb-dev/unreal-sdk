// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Engine/Marketplace/AsyncTaskThirdwebEngineMarketplacePostBase.h"

void UAsyncTaskThirdwebEngineMarketplacePostBase::HandleResponse(const FString& QueueId)
{
	Success.Broadcast(QueueId, TEXT(""));
	SetReadyToDestroy();
}

void UAsyncTaskThirdwebEngineMarketplacePostBase::HandleFailed(const FString& Error)
{
	Failed.Broadcast(TEXT(""), Error);
	SetReadyToDestroy();
}
