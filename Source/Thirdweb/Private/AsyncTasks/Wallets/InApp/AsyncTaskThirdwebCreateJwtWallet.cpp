// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Wallets/InApp/AsyncTaskThirdwebCreateJwtWallet.h"

#include "ThirdwebRuntimeSettings.h"

#include "Wallets/ThirdwebInAppWalletHandle.h"

void UAsyncTaskThirdwebCreateJwtWallet::Activate()
{
	if (UThirdwebRuntimeSettings::IsEcosystem())
	{
		return FInAppWalletHandle::CreateEcosystemJwtWallet(EcosystemPartnerId, BIND_CREATE_WALLET_SUCCESS_DELEGATE, BIND_CREATE_WALLET_ERROR_DELEGATE);
	}
	FInAppWalletHandle::CreateJwtWallet(BIND_CREATE_WALLET_SUCCESS_DELEGATE, BIND_CREATE_WALLET_ERROR_DELEGATE);
}
