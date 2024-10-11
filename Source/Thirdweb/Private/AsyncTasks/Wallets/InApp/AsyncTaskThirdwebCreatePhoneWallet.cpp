// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Wallets/InApp/AsyncTaskThirdwebCreatePhoneWallet.h"

#include "ThirdwebRuntimeSettings.h"

#include "Wallets/ThirdwebInAppWalletHandle.h"

void UAsyncTaskThirdwebCreatePhoneWallet::Activate()
{
	if (UThirdwebRuntimeSettings::IsEcosystem())
	{
		return FInAppWalletHandle::CreateEcosystemPhoneWallet(EcosystemPartnerId, AuthInput,BIND_CREATE_WALLET_SUCCESS_DELEGATE, BIND_CREATE_WALLET_ERROR_DELEGATE);
	}
	FInAppWalletHandle::CreatePhoneWallet(AuthInput,BIND_CREATE_WALLET_SUCCESS_DELEGATE, BIND_CREATE_WALLET_ERROR_DELEGATE);
}
