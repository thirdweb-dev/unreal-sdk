// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Engine/AsyncTaskThirdwebEngineReadContract.h"

#include "Components/SlateWrapperTypes.h"
#include "Engine/ThirdwebEngine.h"

UAsyncTaskThirdwebEngineReadContract* UAsyncTaskThirdwebEngineReadContract::ReadContract(
	UObject* WorldContextObject,
	const int64 ChainID,
	const FString& ContractAddress,
	const FString& FunctionName,
	const TArray<FString>& Args
)
{
	NEW_TASK
	Task->ChainId = ChainID;
	Task->ContractAddress = ContractAddress;
	Task->FunctionName = FunctionName;
	Task->Args = Args;
	RR_TASK
}

void UAsyncTaskThirdwebEngineReadContract::Activate()
{
	ThirdwebEngine::Contract::Read(
		this,
		ChainId,
		ContractAddress,
		FunctionName,
		Args,
		BIND_UOBJECT_DELEGATE(FStringDelegate, HandleResponse),
		BIND_UOBJECT_DELEGATE(FStringDelegate, HandleFailed)
	);
}

void UAsyncTaskThirdwebEngineReadContract::HandleResponse(const FString& Content)
{
	Success.Broadcast(Content, TEXT(""));
	SetReadyToDestroy();
}

void UAsyncTaskThirdwebEngineReadContract::HandleFailed(const FString& Error)
{
	Failed.Broadcast(TEXT(""), Error);
	SetReadyToDestroy();
}
