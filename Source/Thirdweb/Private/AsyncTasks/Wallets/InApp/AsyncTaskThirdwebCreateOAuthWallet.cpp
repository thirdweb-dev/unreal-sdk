// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Wallets/InApp/AsyncTaskThirdwebCreateOAuthWallet.h"

#include "ThirdwebRuntimeSettings.h"

#include "Wallets/ThirdwebInAppWalletHandle.h"

UAsyncTaskThirdwebCreateOAuthWallet* UAsyncTaskThirdwebCreateOAuthWallet::CreateOAuthWallet(UObject* WorldContextObject, const EThirdwebOAuthProvider Provider, const FString& PartnerId)
{
	if (!WorldContextObject)
	{
		return nullptr;
	}
	ThisClass* Task = NewObject<ThisClass>(WorldContextObject);
	Task->EcosystemPartnerId = PartnerId;
	Task->Provider = Provider;
	Task->RegisterWithGameInstance(WorldContextObject);
	return Task;
}

void UAsyncTaskThirdwebCreateOAuthWallet::Activate()
{
	if (UThirdwebRuntimeSettings::IsEcosystem())
	{
		return FInAppWalletHandle::CreateEcosystemOAuthWallet(EcosystemPartnerId, Provider,BIND_CREATE_WALLET_SUCCESS_DELEGATE, BIND_CREATE_WALLET_ERROR_DELEGATE);
	}
	FInAppWalletHandle::CreateOAuthWallet(Provider,BIND_CREATE_WALLET_SUCCESS_DELEGATE, BIND_CREATE_WALLET_ERROR_DELEGATE);
}
