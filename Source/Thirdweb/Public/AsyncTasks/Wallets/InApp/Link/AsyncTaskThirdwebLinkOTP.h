// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "AsyncTaskThirdwebLinkBase.h"
#include "AsyncTaskThirdwebLinkOTP.generated.h"

UCLASS(Abstract)
class THIRDWEB_API UAsyncTaskThirdwebLinkOTP : public UAsyncTaskThirdwebLinkBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject"), DisplayName="Link OTP", Category="Thirdweb|Wallets|In App")
	static UAsyncTaskThirdwebLinkOTP* LinkOTP(UObject* WorldContextObject, const FInAppWalletHandle& Wallet, const FInAppWalletHandle& NewWallet, UPARAM(DisplayName="OTP") const FString& Input)
	{
		LINK_INPUT_TASK
	}

	virtual void Activate() override;
};
