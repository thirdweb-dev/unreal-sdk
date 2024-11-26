// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "AsyncTasks/Engine/Marketplace/AsyncTaskThirdwebEngineMarketplaceBase.h"
#include "Engine/ThirdwebAccountIdentifierParams.h"
#include "AsyncTaskThirdwebEngineMarketplacePostBase.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class UAsyncTaskThirdwebEngineMarketplacePostBase : public UAsyncTaskThirdwebEngineMarketplaceBase
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FMarketplaceQueueDelegate, const FString&, QueueId, const FString&, Error);

	UPROPERTY(BlueprintAssignable)
	FMarketplaceQueueDelegate Success;

	UPROPERTY(BlueprintAssignable)
	FMarketplaceQueueDelegate Failed;

protected:
	UPROPERTY(Transient)
	FString BackendWalletAddress;

	UPROPERTY(Transient)
	FSmartWalletHandle SmartWallet;

	UPROPERTY(Transient)
	FString Idempotency;

	UPROPERTY(Transient)
	bool bSimulateTransaction;
	
	virtual void HandleResponse(const FString& QueueId);
	void HandleFailed(const FString& Error);
};
