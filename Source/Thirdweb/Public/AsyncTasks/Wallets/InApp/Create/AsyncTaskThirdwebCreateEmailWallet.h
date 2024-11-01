// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "AsyncTaskThirdwebInAppCreateWalletBase.h"
#include "AsyncTaskThirdwebCreateEmailWallet.generated.h"

UCLASS(Blueprintable, BlueprintType)
class THIRDWEB_API UAsyncTaskThirdwebCreateEmailWallet : public UAsyncTaskThirdwebInAppCreateWalletBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject"), Category="Thirdweb|Wallets|InApp")
	static UAsyncTaskThirdwebCreateEmailWallet* CreateEmailWallet(UObject* WorldContextObject, UPARAM(DisplayName="Email Address") const FString& Input)
	{
		CREATE_WALLET_INPUT_TASK
	}

	virtual void Activate() override;
};
