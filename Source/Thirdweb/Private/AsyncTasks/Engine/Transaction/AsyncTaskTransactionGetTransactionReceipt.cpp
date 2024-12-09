// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Engine/Transaction/AsyncTaskTransactionGetTransactionReceipt.h"

#include "Components/SlateWrapperTypes.h"
#include "Engine/ThirdwebEngine.h"
#include "Engine/Transaction/ThirdwebEngineTransactionReceipt.h"

UAsyncTaskTransactionGetTransactionReceipt* UAsyncTaskTransactionGetTransactionReceipt::GetTransactionReceipt(UObject* WorldContextObject, const FString& TxHash, const int64 ChainID)
{
	NEW_TASK
	Task->TransactionHash = TxHash;
	Task->ChainId = ChainID;
	RR_TASK
}

void UAsyncTaskTransactionGetTransactionReceipt::Activate()
{
	ThirdwebEngine::Transaction::GetReceipt(
		this,
		TransactionHash,
		ChainId,
		BIND_UOBJECT_DELEGATE(ThirdwebEngine::Transaction::FGetTransactionReceiptDelegate, HandleResponse),
		BIND_UOBJECT_DELEGATE(FStringDelegate, HandleFailed)
	);
}

void UAsyncTaskTransactionGetTransactionReceipt::HandleResponse(const FThirdwebEngineTransactionReceipt& Receipt)
{
	Success.Broadcast(Receipt, TEXT(""));
	SetReadyToDestroy();
}

void UAsyncTaskTransactionGetTransactionReceipt::HandleFailed(const FString& Error)
{
	Failed.Broadcast(FThirdwebEngineTransactionReceipt(), Error);
	SetReadyToDestroy();
}
