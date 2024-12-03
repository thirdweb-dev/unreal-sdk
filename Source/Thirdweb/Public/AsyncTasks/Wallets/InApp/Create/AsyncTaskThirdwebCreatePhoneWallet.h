// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "AsyncTaskThirdwebInAppCreateWalletBase.h"
#include "AsyncTaskThirdwebCreatePhoneWallet.generated.h"

UCLASS(Abstract)
class THIRDWEB_API UAsyncTaskThirdwebCreatePhoneWallet : public UAsyncTaskThirdwebInAppCreateWalletBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject", AdvancedDisplay="PartnerId"), Category="Thirdweb|Wallets|InApp")
	static UAsyncTaskThirdwebCreatePhoneWallet* CreatePhoneWallet(UObject* WorldContextObject, UPARAM(DisplayName="Phone Number") const FString& Input)
	{
		CREATE_WALLET_INPUT_TASK
	}

	virtual void Activate() override;
};
