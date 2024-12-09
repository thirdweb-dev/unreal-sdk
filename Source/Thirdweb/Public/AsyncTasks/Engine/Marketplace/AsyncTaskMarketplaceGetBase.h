// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "AsyncTasks/AsyncTaskThirdwebBase.h"
#include "AsyncTaskMarketplaceGetBase.generated.h"

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
class UAsyncTaskMarketplaceGetBase : public UAsyncTaskThirdwebBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(Transient)
	int64 Chain;

	UPROPERTY(Transient)
	FString MarketplaceContract;
	
};
