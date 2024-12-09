// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Engine/Transaction/AsyncTaskTransactionGetTransactionStatus.h"

#include "Components/SlateWrapperTypes.h"
#include "Engine/ThirdwebEngine.h"
#include "Engine/Transaction/ThirdwebEngineTransactionStatusResult.h"

UAsyncTaskTransactionGetTransactionStatus* UAsyncTaskTransactionGetTransactionStatus::GetTransactionStatus(UObject* WorldContextObject, const FString& QueueID)
{
	NEW_TASK
	Task->QueueId = QueueID;
	RR_TASK
}

void UAsyncTaskTransactionGetTransactionStatus::Activate()
{
	ThirdwebEngine::Transaction::GetStatus(
		this,
		QueueId,
		BIND_UOBJECT_DELEGATE(ThirdwebEngine::Transaction::FGetTransactionStatusDelegate, HandleResponse),
		BIND_UOBJECT_DELEGATE(FStringDelegate, HandleFailed)
	);
}

void UAsyncTaskTransactionGetTransactionStatus::HandleResponse(const FThirdwebEngineTransactionStatusResult& Result)
{
	Success.Broadcast(Result, TEXT(""));
	SetReadyToDestroy();
}

void UAsyncTaskTransactionGetTransactionStatus::HandleFailed(const FString& Error)
{
	Failed.Broadcast(FThirdwebEngineTransactionStatusResult(), Error);
	SetReadyToDestroy();
}
