// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Wallets/InApp/AsyncTaskThirdwebCreateEmailWallet.h"

#include "ThirdwebRuntimeSettings.h"

#include "Wallets/ThirdwebInAppWalletHandle.h"

void UAsyncTaskThirdwebCreateEmailWallet::Activate()
{
	if (UThirdwebRuntimeSettings::IsEcosystem())
	{
		return FInAppWalletHandle::CreateEcosystemEmailWallet(EcosystemPartnerId, AuthInput, BIND_CREATE_WALLET_SUCCESS_DELEGATE, BIND_CREATE_WALLET_ERROR_DELEGATE);
	}
	FInAppWalletHandle::CreateEmailWallet(AuthInput,BIND_CREATE_WALLET_SUCCESS_DELEGATE, BIND_CREATE_WALLET_ERROR_DELEGATE);
}
