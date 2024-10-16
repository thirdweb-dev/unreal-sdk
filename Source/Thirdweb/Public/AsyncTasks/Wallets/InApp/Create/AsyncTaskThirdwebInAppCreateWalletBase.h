// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "AsyncTasks/AsyncTaskThirdwebBase.h"

#include "Components/SlateWrapperTypes.h"

#include "AsyncTaskThirdwebInAppCreateWalletBase.generated.h"

#define BIND_CREATE_WALLET_SUCCESS_DELEGATE BIND_UOBJECT_DELEGATE(FInAppWalletHandle::FCreateInAppWalletDelegate, HandleResponse)
#define BIND_CREATE_WALLET_ERROR_DELEGATE BIND_UOBJECT_DELEGATE(FStringDelegate, HandleFailed)

#define CREATE_WALLET_TASK \
	if (!WorldContextObject) \
	{ \
	return nullptr; \
	} \
	ThisClass* Task = NewObject<ThisClass>(WorldContextObject); \
	Task->EcosystemPartnerId = PartnerId; \
	Task->RegisterWithGameInstance(WorldContextObject); \
	return Task;

#define CREATE_WALLET_INPUT_TASK \
	if (!WorldContextObject) \
		{ \
			return nullptr; \
		} \
	ThisClass* Task = NewObject<ThisClass>(WorldContextObject); \
	Task->AuthInput = Input; \
	Task->EcosystemPartnerId = PartnerId; \
	Task->RegisterWithGameInstance(WorldContextObject); \
	return Task;

struct FInAppWalletHandle;

UCLASS(Abstract)
class UAsyncTaskThirdwebInAppCreateWalletBase : public UAsyncTaskThirdwebBase
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCreateInAppWalletDelegate, const FInAppWalletHandle&, Wallet, const FString&, Error);

	UPROPERTY(BlueprintAssignable)
	FCreateInAppWalletDelegate Success;

	UPROPERTY(BlueprintAssignable)
	FCreateInAppWalletDelegate Failed;

protected:
	UPROPERTY(Transient)
	FString AuthInput;

	UPROPERTY(Transient)
	FString EcosystemPartnerId;

	UFUNCTION()
	void HandleResponse(const FInAppWalletHandle& Wallet);

	UFUNCTION()
	virtual void HandleFailed(const FString& Error);
};
