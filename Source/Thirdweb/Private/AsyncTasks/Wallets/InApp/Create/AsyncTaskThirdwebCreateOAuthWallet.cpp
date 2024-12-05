// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Wallets/InApp/Create/AsyncTaskThirdwebCreateOAuthWallet.h"

#include "ThirdwebRuntimeSettings.h"
#include "Wallets/ThirdwebInAppWalletHandle.h"

void UAsyncTaskThirdwebCreateOAuthWallet::Activate()
{
	FInAppWalletHandle::CreateOAuthWallet(Provider,BIND_CREATE_WALLET_SUCCESS_DELEGATE, BIND_CREATE_WALLET_ERROR_DELEGATE);
}
