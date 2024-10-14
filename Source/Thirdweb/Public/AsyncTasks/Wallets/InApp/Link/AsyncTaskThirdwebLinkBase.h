// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "AsyncTasks/Wallets/InApp/AsyncTaskThirdwebInAppBase.h"

#include "AsyncTaskThirdwebLinkBase.generated.h"

#define LINK_TASK \
	if (!WorldContextObject) \
	{ \
		return nullptr; \
	} \
	ThisClass* Task = NewObject<ThisClass>(WorldContextObject); \
	Task->InAppWallet = Wallet; \
	Task->NewInAppWallet = NewWallet; \
	Task->RegisterWithGameInstance(WorldContextObject); \
	return Task;

#define LINK_INPUT_TASK \
	if (!WorldContextObject) \
	{ \
		return nullptr; \
	} \
	ThisClass* Task = NewObject<ThisClass>(WorldContextObject); \
	Task->InAppWallet = Wallet; \
	Task->NewInAppWallet = NewWallet; \
	Task->AuthInput = Input; \
	Task->RegisterWithGameInstance(WorldContextObject); \
	return Task;

UCLASS(Abstract)
class THIRDWEB_API UAsyncTaskThirdwebLinkBase : public UAsyncTaskThirdwebInAppBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FErrorOnlyDelegate Success;

	UPROPERTY(BlueprintAssignable)
	FErrorOnlyDelegate Failed;

protected:
	UPROPERTY(Transient)
	FInAppWalletHandle NewInAppWallet;
	
	UPROPERTY(Transient)
	FString AuthInput;
	
	UFUNCTION()
	void HandleResponse();

	UFUNCTION()
	virtual void HandleFailed(const FString& Error);
};
