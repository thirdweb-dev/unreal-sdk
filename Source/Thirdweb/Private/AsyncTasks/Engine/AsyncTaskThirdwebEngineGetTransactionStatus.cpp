// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Engine/AsyncTaskThirdwebEngineGetTransactionStatus.h"

#include "Components/SlateWrapperTypes.h"
#include "Engine/ThirdwebEngine.h"
#include "Engine/Transaction/ThirdwebEngineTransactionStatusResult.h"

UAsyncTaskThirdwebEngineGetTransactionStatus* UAsyncTaskThirdwebEngineGetTransactionStatus::GetTransactionStatus(UObject* WorldContextObject, const FString& QueueID)
{
	NEW_TASK
	Task->QueueId = QueueID;
	RR_TASK
}

void UAsyncTaskThirdwebEngineGetTransactionStatus::Activate()
{
	ThirdwebEngine::Transaction::GetStatus(
		this,
		QueueId,
		BIND_UOBJECT_DELEGATE(ThirdwebEngine::Transaction::FGetTransactionStatusDelegate, HandleResponse),
		BIND_UOBJECT_DELEGATE(FStringDelegate, HandleFailed)
	);
}

void UAsyncTaskThirdwebEngineGetTransactionStatus::HandleResponse(const FThirdwebEngineTransactionStatusResult& Result)
{
	Success.Broadcast(Result, TEXT(""));
	SetReadyToDestroy();
}

void UAsyncTaskThirdwebEngineGetTransactionStatus::HandleFailed(const FString& Error)
{
	Failed.Broadcast(FThirdwebEngineTransactionStatusResult(), Error);
	SetReadyToDestroy();
}
