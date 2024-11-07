// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "AsyncTasks/AsyncTaskThirdwebBase.h"

#include "Interfaces/IHttpRequest.h"

#include "AsyncTaskThirdwebEngineGetTransactionStatus.generated.h"

struct FThirdwebEngineTransactionStatusResult;
class FJsonObject;

UCLASS(Blueprintable, BlueprintType)
class THIRDWEB_API UAsyncTaskThirdwebEngineGetTransactionStatus : public UAsyncTaskThirdwebBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject"), Category="Thirdweb|Engine")
	static UAsyncTaskThirdwebEngineGetTransactionStatus* GetTransactionStatus(UObject* WorldContextObject, const FString& QueueID);

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
	virtual void HandleResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
	void HandleFailed(const FString& Error);
};
