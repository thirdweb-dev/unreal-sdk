// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Wallets/Smart/AsyncTaskThirdwebGetAdmins.h"

#include "ThirdwebSigner.h"

#include "Components/SlateWrapperTypes.h"

void UAsyncTaskThirdwebGetAdmins::Activate()
{
	SmartWallet.GetAdmins(BIND_UOBJECT_DELEGATE(FStringArrayDelegate, HandleResponse), BIND_UOBJECT_DELEGATE(FStringDelegate, HandleFailed));
}

UAsyncTaskThirdwebGetAdmins* UAsyncTaskThirdwebGetAdmins::GetAdmins(UObject* WorldContextObject, const FSmartWalletHandle& Wallet)
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

void UAsyncTaskThirdwebGetAdmins::HandleResponse(const TArray<FString>& Admins)
{
	Success.Broadcast(Admins, TEXT(""));
	SetReadyToDestroy();
}

void UAsyncTaskThirdwebGetAdmins::HandleFailed(const FString& Error)
{
	Failed.Broadcast({}, Error);
	SetReadyToDestroy();
}
