// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Wallets/InApp/Create/AsyncTaskThirdwebCreateJwtWallet.h"

#include "Wallets/ThirdwebInAppWalletHandle.h"

void UAsyncTaskThirdwebCreateJwtWallet::Activate()
{
	FInAppWalletHandle::CreateJwtWallet(BIND_CREATE_WALLET_SUCCESS_DELEGATE, BIND_CREATE_WALLET_ERROR_DELEGATE);
}
