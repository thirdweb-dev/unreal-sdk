// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Wallets/InApp/AsyncTaskThirdwebCreateSmartWallet.h"

#include "Components/SlateWrapperTypes.h"

#include "Wallets/ThirdwebSmartWalletHandle.h"

void UAsyncTaskThirdwebCreateSmartWallet::Activate()
{
	FSmartWalletHandle::Create(
		InAppWallet,
		ChainID,
		bGasless,
		Factory,
		AccountOverride,
		BIND_UOBJECT_DELEGATE(FSmartWalletHandle::FCreateSmartWalletDelegate, HandleResponse),
		BIND_UOBJECT_DELEGATE(FStringDelegate, HandleFailed)
		);

}

UAsyncTaskThirdwebCreateSmartWallet* UAsyncTaskThirdwebCreateSmartWallet::CreateSmartWallet(UObject* WorldContextObject,
	const FInAppWalletHandle& InAppWallet,
	const int64 ChainID,
	const bool bGasless,
	const FString& Factory,
	const FString& AccountOverride)
{
	if (!WorldContextObject)
	{
		return nullptr;
	}
	NEW_TASK

	Task->InAppWallet = InAppWallet;
	Task->ChainID = ChainID;
	Task->bGasless = bGasless;
	Task->Factory = Factory;
	Task->AccountOverride = AccountOverride;

	Task->RegisterWithGameInstance(WorldContextObject);
	return Task;
}

void UAsyncTaskThirdwebCreateSmartWallet::HandleResponse(const FSmartWalletHandle& Wallet)
{
	Success.Broadcast(Wallet, TEXT(""));
	SetReadyToDestroy();
}

void UAsyncTaskThirdwebCreateSmartWallet::HandleFailed(const FString& Error)
{
	Failed.Broadcast(FSmartWalletHandle(), Error);
	SetReadyToDestroy();
}
