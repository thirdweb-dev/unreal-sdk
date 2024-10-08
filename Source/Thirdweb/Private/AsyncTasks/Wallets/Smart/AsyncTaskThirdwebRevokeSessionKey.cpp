// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Wallets/Smart/AsyncTaskThirdwebRevokeSessionKey.h"

#include "ThirdwebSigner.h"

#include "Components/SlateWrapperTypes.h"

void UAsyncTaskThirdwebRevokeSessionKey::Activate()
{
	FString Error;
	SmartWallet.RevokeSessionKey(Signer, BIND_UOBJECT_DELEGATE(FSimpleDelegate, HandleResponse), BIND_UOBJECT_DELEGATE(FStringDelegate, HandleFailed));
}

UAsyncTaskThirdwebRevokeSessionKey* UAsyncTaskThirdwebRevokeSessionKey::RevokeSessionKey(UObject* WorldContextObject, const FSmartWalletHandle& Wallet, const FString& Signer)
{
	if (!WorldContextObject)
	{
		return nullptr;
	}
	NEW_TASK
	Task->SmartWallet = Wallet;
	Task->Signer = Signer;
	Task->RegisterWithGameInstance(WorldContextObject);
	return Task;
}

void UAsyncTaskThirdwebRevokeSessionKey::HandleResponse()
{
	Success.Broadcast(TEXT(""));
	return SetReadyToDestroy();
}

void UAsyncTaskThirdwebRevokeSessionKey::HandleFailed(const FString& Error)
{
	Failed.Broadcast(Error);
	SetReadyToDestroy();
}
