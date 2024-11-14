// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "AsyncTaskThirdwebInAppCreateWalletBase.h"
#include "AsyncTaskThirdwebCreateAuthEndpointWallet.generated.h"

UCLASS(Abstract)
class THIRDWEB_API UAsyncTaskThirdwebCreateAuthEndpointWallet : public UAsyncTaskThirdwebInAppCreateWalletBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject"), Category="Thirdweb|Wallets|InApp")
	static UAsyncTaskThirdwebCreateAuthEndpointWallet* CreateAuthEndpointWallet(UObject* WorldContextObject) { CREATE_WALLET_TASK }

	virtual void Activate() override;
};
