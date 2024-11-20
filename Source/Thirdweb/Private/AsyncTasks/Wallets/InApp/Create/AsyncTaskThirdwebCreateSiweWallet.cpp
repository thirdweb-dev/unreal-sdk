// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Wallets/InApp/Create/AsyncTaskThirdwebCreateSiweWallet.h"

#include "Wallets/ThirdwebInAppWalletHandle.h"

void UAsyncTaskThirdwebCreateSiweWallet::Activate()
{
	FInAppWalletHandle::CreateSiweWallet(BIND_CREATE_WALLET_SUCCESS_DELEGATE, BIND_CREATE_WALLET_ERROR_DELEGATE);
}
