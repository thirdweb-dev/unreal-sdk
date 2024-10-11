// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "AsyncTaskThirdwebInAppBase.h"
#include "AsyncTaskThirdwebInAppCreateWalletBase.h"

#include "Wallets/ThirdwebInAppWalletHandle.h"

#include "AsyncTaskThirdwebCreateAuthEndpointWallet.generated.h"

UCLASS(Blueprintable, BlueprintType)
class THIRDWEB_API UAsyncTaskThirdwebCreateAuthEndpointWallet : public UAsyncTaskThirdwebInAppCreateWalletBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject", AdvancedDisplay="PartnerId"), Category="Thirdweb|Wallets|InApp")
	static UAsyncTaskThirdwebCreateAuthEndpointWallet* CreateAuthEndpointWallet(UObject* WorldContextObject, const FString& PartnerId) { CREATE_WALLET_TASK }

	virtual void Activate() override;
};
