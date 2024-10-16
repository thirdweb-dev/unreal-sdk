// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "AsyncTaskThirdwebLinkBase.h"
#include "AsyncTaskThirdwebLinkJwt.generated.h"

UCLASS(Blueprintable, BlueprintType)
class THIRDWEB_API UAsyncTaskThirdwebLinkJwt : public UAsyncTaskThirdwebLinkBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject"), DisplayName="Link JWT", Category="Thirdweb|Wallets|In App")
	static UAsyncTaskThirdwebLinkJwt* LinkJwt(UObject* WorldContextObject, const FInAppWalletHandle& Wallet, const FInAppWalletHandle& NewWallet, UPARAM(DisplayName="JWT") const FString& Input)
	{
		LINK_INPUT_TASK
	}

	virtual void Activate() override;
};
