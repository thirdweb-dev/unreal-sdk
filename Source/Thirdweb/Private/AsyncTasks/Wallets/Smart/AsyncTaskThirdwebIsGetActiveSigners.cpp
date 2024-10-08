// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "ThirdwebSigner.h"

#include "AsyncTasks/Wallets/Smart/AsyncTaskThirdwebGetActiveSigners.h"

#include "Components/SlateWrapperTypes.h"

void UAsyncTaskThirdwebGetActiveSigners::Activate()
{
	FString Error;
	SmartWallet.IsDeployed(BIND_UOBJECT_DELEGATE(FBoolDelegate, HandleIsDeployedResponse), BIND_UOBJECT_DELEGATE(FStringDelegate, HandleFailed));
}

UAsyncTaskThirdwebGetActiveSigners* UAsyncTaskThirdwebGetActiveSigners::GetActiveSigners(UObject* WorldContextObject, const FSmartWalletHandle& Wallet)
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

void UAsyncTaskThirdwebGetActiveSigners::HandleIsDeployedResponse(const bool& bDeployed)
{
	if (bDeployed)
	{
		return SmartWallet.GetActiveSigners(BIND_UOBJECT_DELEGATE(FSmartWalletHandle::FGetActiveSignersDelegate, HandleResponse), BIND_UOBJECT_DELEGATE(FStringDelegate, HandleFailed));
	}
	Success.Broadcast({}, TEXT(""));
	SetReadyToDestroy();
}

void UAsyncTaskThirdwebGetActiveSigners::HandleResponse(const TArray<FSigner>& Signers)
{
	Success.Broadcast(Signers, TEXT(""));
	SetReadyToDestroy();
}

void UAsyncTaskThirdwebGetActiveSigners::HandleFailed(const FString& Error)
{
	Failed.Broadcast({}, Error);
	SetReadyToDestroy();
}
