// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "AsyncTaskThirdwebInAppCreateWalletBase.h"
#include "AsyncTaskThirdwebCreateSiweWallet.generated.h"

UCLASS(Abstract)
class THIRDWEB_API UAsyncTaskThirdwebCreateSiweWallet : public UAsyncTaskThirdwebInAppCreateWalletBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, DisplayName="Create SIWE Wallet", meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject"), Category="Thirdweb|Wallets|InApp")
	static UAsyncTaskThirdwebCreateSiweWallet* CreateSiweWallet(UObject* WorldContextObject) { CREATE_WALLET_TASK }

	virtual void Activate() override;
};
