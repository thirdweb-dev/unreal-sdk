// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "AsyncTasks/AsyncTaskThirdwebBase.h"
#include "Engine/Transaction/ThirdwebEngineTransactionReceipt.h"
#include "AsyncTaskTransactionGetTransactionReceipt.generated.h"


UCLASS(Blueprintable, BlueprintType)
class THIRDWEB_API UAsyncTaskTransactionGetTransactionReceipt : public UAsyncTaskThirdwebBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject"), Category="Thirdweb|Engine|Transaction")
	static UAsyncTaskTransactionGetTransactionReceipt* GetTransactionReceipt(UObject* WorldContextObject, const FString& TxHash, const int64 ChainID);

	virtual void Activate() override;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGetTransactionReceiptDelegate, const FThirdwebEngineTransactionReceipt&, Result, const FString&, Error);

	UPROPERTY(BlueprintAssignable)
	FGetTransactionReceiptDelegate Success;

	UPROPERTY(BlueprintAssignable)
	FGetTransactionReceiptDelegate Failed;

protected:
	UPROPERTY(Transient)
	FString TransactionHash;

	UPROPERTY(Transient)
	int64 ChainId;

private:
	virtual void HandleResponse(const FThirdwebEngineTransactionReceipt& Receipt);
	void HandleFailed(const FString& Error);
};
