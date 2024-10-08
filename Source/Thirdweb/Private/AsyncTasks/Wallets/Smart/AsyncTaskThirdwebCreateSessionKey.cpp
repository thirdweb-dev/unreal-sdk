// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Wallets/Smart/AsyncTaskThirdwebCreateSessionKey.h"

#include "Components/SlateWrapperTypes.h"

void UAsyncTaskThirdwebCreateSessionKey::Activate()
{
	SmartWallet.CreateSessionKey(
		Signer,
		ApprovedTargets,
		NativeTokenLimitPerTransactionInWei,
		PermissionStart,
		PermissionEnd,
		RequestValidityStart,
		RequestValidityEnd,
		BIND_UOBJECT_DELEGATE(FStringDelegate, HandleResponse),
		BIND_UOBJECT_DELEGATE(FStringDelegate, HandleFailed)
	);
}

UAsyncTaskThirdwebCreateSessionKey* UAsyncTaskThirdwebCreateSessionKey::CreateSessionKey(UObject* WorldContextObject,
                                                                                         const FSmartWalletHandle& Wallet,
                                                                                         const FString& Signer,
                                                                                         const TArray<FString>& ApprovedTargets,
                                                                                         const FString& NativeTokenLimitPerTransactionInWei,
                                                                                         const FDateTime& PermissionStart,
                                                                                         const FDateTime& PermissionEnd,
                                                                                         const FDateTime& RequestValidityStart,
                                                                                         const FDateTime& RequestValidityEnd)
{
	if (!WorldContextObject)
	{
		return nullptr;
	}
	NEW_TASK

	Task->SmartWallet = Wallet;
	Task->Signer = Signer;
	Task->ApprovedTargets = ApprovedTargets;
	Task->NativeTokenLimitPerTransactionInWei = NativeTokenLimitPerTransactionInWei;
	Task->PermissionStart = PermissionStart;
	Task->PermissionEnd = PermissionEnd;
	Task->RequestValidityStart = RequestValidityStart;
	Task->RequestValidityEnd = RequestValidityEnd;

	Task->RegisterWithGameInstance(WorldContextObject);
	return Task;
}

void UAsyncTaskThirdwebCreateSessionKey::HandleResponse(const FString& TxHash)
{
	Success.Broadcast(TxHash, TEXT(""));
	SetReadyToDestroy();
}

void UAsyncTaskThirdwebCreateSessionKey::HandleFailed(const FString& Error)
{
	Failed.Broadcast(TEXT(""), Error);
	SetReadyToDestroy();
}
