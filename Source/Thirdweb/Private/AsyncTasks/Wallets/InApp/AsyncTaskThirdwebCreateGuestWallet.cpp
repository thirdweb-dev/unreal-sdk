// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Wallets/InApp/AsyncTaskThirdwebCreateGuestWallet.h"

#include "ThirdwebRuntimeSettings.h"

#include "Wallets/ThirdwebInAppWalletHandle.h"

void UAsyncTaskThirdwebCreateGuestWallet::Activate()
{
	if (UThirdwebRuntimeSettings::IsEcosystem())
	{
		return FInAppWalletHandle::CreateEcosystemGuestWallet(EcosystemPartnerId, BIND_CREATE_WALLET_SUCCESS_DELEGATE, BIND_CREATE_WALLET_ERROR_DELEGATE);
	}
	FInAppWalletHandle::CreateGuestWallet(BIND_CREATE_WALLET_SUCCESS_DELEGATE, BIND_CREATE_WALLET_ERROR_DELEGATE);
}
