// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Wallets/InApp/Create/AsyncTaskThirdwebCreateOAuthWallet.h"

#include "ThirdwebRuntimeSettings.h"

#include "Wallets/ThirdwebInAppWalletHandle.h"

UAsyncTaskThirdwebCreateOAuthWallet* UAsyncTaskThirdwebCreateOAuthWallet::CreateOAuthWallet(UObject* WorldContextObject, const EThirdwebOAuthProvider Provider)
{
	if (!WorldContextObject)
	{
		return nullptr;
	}
	ThisClass* Task = NewObject<ThisClass>(WorldContextObject);
	Task->Provider = Provider;
	Task->RegisterWithGameInstance(WorldContextObject);
	return Task;
}

void UAsyncTaskThirdwebCreateOAuthWallet::Activate()
{
	FInAppWalletHandle::CreateOAuthWallet(Provider,BIND_CREATE_WALLET_SUCCESS_DELEGATE, BIND_CREATE_WALLET_ERROR_DELEGATE);
}
