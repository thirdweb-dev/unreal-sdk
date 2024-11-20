// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Engine/AsyncTaskThirdwebEngineWriteContract.h"

#include "Components/SlateWrapperTypes.h"
#include "Engine/ThirdwebEngine.h"

UAsyncTaskThirdwebEngineWriteContract* UAsyncTaskThirdwebEngineWriteContract::WriteContract(
	UObject* WorldContextObject,
	const int64 ChainId,
	const FString& ContractAddress,
	const FString& BackendWalletAddress,
	const FSmartWalletHandle& SmartWallet,
	const FString& FactoryAddress,
	const FString& IdempotencyKey,
	const FString& FunctionName,
	const TArray<FString>& Args,
	const FThirdwebEngineTransactionOverrides& TxOverrides,
	const FString& Abi,
	const bool bSimulateTx)
{
	NEW_TASK
	Task->ChainId = ChainId;
	Task->ContractAddress = ContractAddress.TrimStartAndEnd();
	Task->BackendWalletAddress = BackendWalletAddress.TrimStartAndEnd();
	Task->SmartWallet = SmartWallet;
	Task->FactoryAddress = FactoryAddress.TrimStartAndEnd();
	Task->IdempotencyKey = IdempotencyKey.TrimStartAndEnd();
	Task->FunctionName = FunctionName.TrimStartAndEnd();
	Task->Args = Args;
	Task->TxOverrides = TxOverrides;
	Task->Abi = Abi.TrimStartAndEnd();
	Task->bSimulateTx = bSimulateTx;
	RR_TASK
}

void UAsyncTaskThirdwebEngineWriteContract::Activate()
{
	ThirdwebEngine::Contract::Write(
		this,
		ChainId,
		ContractAddress,
		BackendWalletAddress,
		SmartWallet,
		FactoryAddress,
		IdempotencyKey,
		FunctionName,
		Args,
		TxOverrides,
		Abi,
		bSimulateTx,
		BIND_UOBJECT_DELEGATE(FStringDelegate, HandleResponse),
		BIND_UOBJECT_DELEGATE(FStringDelegate, HandleFailed)
	);
}

void UAsyncTaskThirdwebEngineWriteContract::HandleResponse(const FString& QueueId)
{
	Success.Broadcast(QueueId, TEXT(""));
	SetReadyToDestroy();
}

void UAsyncTaskThirdwebEngineWriteContract::HandleFailed(const FString& Error)
{
	Failed.Broadcast(TEXT(""), Error);
	SetReadyToDestroy();
}
