// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "AsyncTasks/Wallets/InApp/AsyncTaskThirdwebInAppBase.h"
#include "AsyncTaskThirdwebSignInBase.generated.h"

#define SIGN_IN_TASK \
	if (!WorldContextObject) \
	{ \
		return nullptr; \
	} \
	ThisClass* Task = NewObject<ThisClass>(WorldContextObject); \
	Task->InAppWallet = Wallet; \
	Task->RegisterWithGameInstance(WorldContextObject); \
	return Task;

#define SIGN_IN_INPUT_TASK \
	if (!WorldContextObject) \
	{ \
		return nullptr; \
	} \
	ThisClass* Task = NewObject<ThisClass>(WorldContextObject); \
	Task->InAppWallet = Wallet; \
	Task->AuthInput = Input; \
	Task->RegisterWithGameInstance(WorldContextObject); \
	return Task;

UCLASS(Abstract)
class THIRDWEB_API UAsyncTaskThirdwebSignInBase : public UAsyncTaskThirdwebInAppBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FErrorOnlyDelegate Success;

	UPROPERTY(BlueprintAssignable)
	FErrorOnlyDelegate Failed;

protected:
	UPROPERTY(Transient)
	FString AuthInput;
	
	UFUNCTION()
	void HandleResponse();

	UFUNCTION()
	virtual void HandleFailed(const FString& Error);
};
