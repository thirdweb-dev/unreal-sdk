// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Wallets/Smart/AsyncTaskThirdwebIsActiveSigner.h"

#include "ThirdwebSigner.h"

#include "Components/SlateWrapperTypes.h"

void UAsyncTaskThirdwebIsActiveSigner::Activate()
{
	FString Error;
	SmartWallet.IsDeployed(BIND_UOBJECT_DELEGATE(FBoolDelegate, HandleIsDeployedResponse), BIND_UOBJECT_DELEGATE(FStringDelegate, HandleFailed));
}

UAsyncTaskThirdwebIsActiveSigner* UAsyncTaskThirdwebIsActiveSigner::IsActiveSigner(UObject* WorldContextObject, const FSmartWalletHandle& Wallet, const FString& BackendWallet)
{
	if (!WorldContextObject)
	{
		return nullptr;
	}
	NEW_TASK
	Task->SmartWallet = Wallet;
	Task->BackendWallet = BackendWallet;
	Task->RegisterWithGameInstance(WorldContextObject);
	return Task;
}

void UAsyncTaskThirdwebIsActiveSigner::HandleIsDeployedResponse(const bool& bDeployed)
{
	if (bDeployed)
	{
		return SmartWallet.GetActiveSigners(BIND_UOBJECT_DELEGATE(FSmartWalletHandle::FGetActiveSignersDelegate, HandleResponse), BIND_UOBJECT_DELEGATE(FStringDelegate, HandleFailed));
	}
	Success.Broadcast(false, TEXT(""));
	SetReadyToDestroy();
}

void UAsyncTaskThirdwebIsActiveSigner::HandleResponse(const TArray<FSigner>& Signers)
{
	for (int i = 0; i < Signers.Num(); i++)
	{
		if (Signers[i].Address.Equals(BackendWallet, ESearchCase::IgnoreCase))
		{
			Success.Broadcast(true, TEXT(""));
			return SetReadyToDestroy();
		}
	}
}

void UAsyncTaskThirdwebIsActiveSigner::HandleFailed(const FString& Error)
{
	Failed.Broadcast(false, Error);
	SetReadyToDestroy();
}
