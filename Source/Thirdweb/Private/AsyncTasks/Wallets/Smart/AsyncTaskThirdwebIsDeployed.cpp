// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Wallets/Smart/AsyncTaskThirdwebIsDeployed.h"

#include "ThirdwebSigner.h"

#include "Components/SlateWrapperTypes.h"

void UAsyncTaskThirdwebIsDeployed::Activate()
{
	FString Error;
	SmartWallet.IsDeployed(BIND_UOBJECT_DELEGATE(FBoolDelegate, HandleResponse), BIND_UOBJECT_DELEGATE(FStringDelegate, HandleFailed));
}

UAsyncTaskThirdwebIsDeployed* UAsyncTaskThirdwebIsDeployed::IsDeployed(UObject* WorldContextObject, const FSmartWalletHandle& Wallet)
{
	if (!WorldContextObject)
	{
		return nullptr;
	}
	NEW_TASK
	Task->SmartWallet = Wallet;
	Task->RegisterWithGameInstance(WorldContextObject);
	return Task;
}


void UAsyncTaskThirdwebIsDeployed::HandleResponse(const bool& bIsDeployed)
{
	if (bIsDeployed)
	{
		Deployed.Broadcast(TEXT(""));
	} else
	{
		NotDeployed.Broadcast(TEXT(""));
	}
	SetReadyToDestroy();
}

void UAsyncTaskThirdwebIsDeployed::HandleFailed(const FString& Error)
{
	Failed.Broadcast(Error);
	SetReadyToDestroy();
}