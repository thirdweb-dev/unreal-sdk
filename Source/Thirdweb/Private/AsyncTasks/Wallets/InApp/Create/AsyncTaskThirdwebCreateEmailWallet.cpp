// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Wallets/InApp/Create/AsyncTaskThirdwebCreateEmailWallet.h"
#include "Wallets/ThirdwebInAppWalletHandle.h"

void UAsyncTaskThirdwebCreateEmailWallet::Activate()
{
	FInAppWalletHandle::CreateEmailWallet(AuthInput,BIND_CREATE_WALLET_SUCCESS_DELEGATE, BIND_CREATE_WALLET_ERROR_DELEGATE);
}
