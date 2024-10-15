// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Wallets/InApp/AsyncTaskThirdwebGetLinkedAccounts.h"

#include "Components/SlateWrapperTypes.h"

void UAsyncTaskThirdwebGetLinkedAccounts::Activate()
{
	InAppWallet.GetLinkedAccounts(BIND_UOBJECT_DELEGATE(FInAppWalletHandle::FGetLinkedAccountsDelegate, HandleResponse), BIND_UOBJECT_DELEGATE(FStringDelegate, HandleFailed));
}

UAsyncTaskThirdwebGetLinkedAccounts* UAsyncTaskThirdwebGetLinkedAccounts::GetLinkedAccounts(UObject* WorldContextObject, const FInAppWalletHandle& Wallet)
{
	if (!WorldContextObject)
	{
		return nullptr;
	}
	NEW_TASK
	Task->InAppWallet = Wallet;
	Task->RegisterWithGameInstance(WorldContextObject);
	return Task;
}

void UAsyncTaskThirdwebGetLinkedAccounts::HandleResponse(const TArray<FString>& LinkedAccounts)
{
	Success.Broadcast(LinkedAccounts, TEXT(""));
	SetReadyToDestroy();
}

void UAsyncTaskThirdwebGetLinkedAccounts::HandleFailed(const FString& Error)
{
	Failed.Broadcast({}, Error);
	SetReadyToDestroy();
}
