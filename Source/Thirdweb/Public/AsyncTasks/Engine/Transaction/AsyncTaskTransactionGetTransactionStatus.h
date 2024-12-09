// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "AsyncTasks/AsyncTaskThirdwebBase.h"
#include "AsyncTaskTransactionGetTransactionStatus.generated.h"

struct FThirdwebEngineTransactionStatusResult;

UCLASS(Blueprintable, BlueprintType)
class THIRDWEB_API UAsyncTaskTransactionGetTransactionStatus : public UAsyncTaskThirdwebBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject"), Category="Thirdweb|Engine|Transaction")
	static UAsyncTaskTransactionGetTransactionStatus* GetTransactionStatus(UObject* WorldContextObject, const FString& QueueID);

	virtual void Activate() override;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGetTransactionStatusDelegate, const FThirdwebEngineTransactionStatusResult&, Result, const FString&, Error);

	UPROPERTY(BlueprintAssignable)
	FGetTransactionStatusDelegate Success;

	UPROPERTY(BlueprintAssignable)
	FGetTransactionStatusDelegate Failed;

protected:
	UPROPERTY(Transient)
	FString QueueId;

private:
	virtual void HandleResponse(const FThirdwebEngineTransactionStatusResult& Result);
	void HandleFailed(const FString& Error);
};
