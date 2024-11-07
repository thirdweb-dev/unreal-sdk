// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Wallets/InApp/Create/AsyncTaskThirdwebCreateGuestWallet.h"
#include "Wallets/ThirdwebInAppWalletHandle.h"

void UAsyncTaskThirdwebCreateGuestWallet::Activate()
{
	FInAppWalletHandle::CreateGuestWallet(BIND_CREATE_WALLET_SUCCESS_DELEGATE, BIND_CREATE_WALLET_ERROR_DELEGATE);
}
