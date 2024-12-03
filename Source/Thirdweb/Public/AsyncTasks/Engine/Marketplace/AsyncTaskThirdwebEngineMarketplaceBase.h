// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "AsyncTasks/AsyncTaskThirdwebBase.h"
#include "AsyncTaskThirdwebEngineMarketplaceBase.generated.h"

#define NEW_MARKETPLACE_TASK \
	if (!WorldContextObject) \
	{ \
		return nullptr; \
	} \
	if (!Marketplace) { \
		return nullptr; \
	} \
	ThisClass* Task = NewObject<ThisClass>(WorldContextObject);

/**
 * 
 */
UCLASS(Abstract)
class UAsyncTaskThirdwebEngineMarketplaceBase : public UAsyncTaskThirdwebBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(Transient)
	int64 Chain;

	UPROPERTY(Transient)
	FString MarketplaceContract;
	
};
