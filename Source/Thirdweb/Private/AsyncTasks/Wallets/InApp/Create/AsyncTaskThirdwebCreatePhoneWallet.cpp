// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Wallets/InApp/Create/AsyncTaskThirdwebCreatePhoneWallet.h"

#include "Wallets/ThirdwebInAppWalletHandle.h"

void UAsyncTaskThirdwebCreatePhoneWallet::Activate()
{
	FInAppWalletHandle::CreatePhoneWallet(AuthInput,BIND_CREATE_WALLET_SUCCESS_DELEGATE, BIND_CREATE_WALLET_ERROR_DELEGATE);
}
