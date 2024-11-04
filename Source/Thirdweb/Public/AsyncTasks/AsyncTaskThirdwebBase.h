// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintAsyncActionBase.h"

#include "AsyncTaskThirdwebBase.generated.h"

#define NEW_TASK ThisClass* Task = NewObject<ThisClass>(WorldContextObject);
#define RR_TASK \
	Task->RegisterWithGameInstance(WorldContextObject); \
	return Task;

UCLASS(Abstract)
class UAsyncTaskThirdwebBase : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FErrorOnlyDelegate, const FString&, Error);
	virtual void Activate() override;
};
