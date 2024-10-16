// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "AsyncTaskThirdwebLinkBase.h"
#include "AsyncTaskThirdwebLinkOAuth.generated.h"

UCLASS(Blueprintable, BlueprintType)
class THIRDWEB_API UAsyncTaskThirdwebLinkOAuth : public UAsyncTaskThirdwebLinkBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject"), DisplayName="Link OAuth", Category="Thirdweb|Wallets|In App")
	static UAsyncTaskThirdwebLinkOAuth* LinkOAuth(UObject* WorldContextObject,
	                                              const FInAppWalletHandle& Wallet,
	                                              const FInAppWalletHandle& NewWallet,
	                                              UPARAM(DisplayName="Auth Result") const FString& Input)
	{
		LINK_INPUT_TASK
	}

	virtual void Activate() override;
};
