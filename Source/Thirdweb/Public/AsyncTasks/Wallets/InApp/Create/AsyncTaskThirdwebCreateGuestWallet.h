// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "AsyncTaskThirdwebInAppCreateWalletBase.h"
#include "AsyncTaskThirdwebCreateGuestWallet.generated.h"

UCLASS(Blueprintable, BlueprintType)
class THIRDWEB_API UAsyncTaskThirdwebCreateGuestWallet : public UAsyncTaskThirdwebInAppCreateWalletBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject"), Category="Thirdweb|Wallets|InApp")
	static UAsyncTaskThirdwebCreateGuestWallet* CreateGuestWallet(UObject* WorldContextObject) { CREATE_WALLET_TASK }

	virtual void Activate() override;
};
