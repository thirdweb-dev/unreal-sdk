// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Wallets/InApp/Create/AsyncTaskThirdwebCreateAuthEndpointWallet.h"
#include "Wallets/ThirdwebInAppWalletHandle.h"

void UAsyncTaskThirdwebCreateAuthEndpointWallet::Activate()
{
	FInAppWalletHandle::CreateAuthEndpointWallet(BIND_CREATE_WALLET_SUCCESS_DELEGATE, BIND_CREATE_WALLET_ERROR_DELEGATE);
}
