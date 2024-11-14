// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "AsyncTaskThirdwebLinkBase.h"
#include "AsyncTaskThirdwebLink.generated.h"

UCLASS(Abstract)
class THIRDWEB_API UAsyncTaskThirdwebLink : public UAsyncTaskThirdwebLinkBase
{
	GENERATED_BODY()

public:
	/** Multi-Purpose runtime link node covering all link options based upon Wallet. Input is ignored if guest wallet */
	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject"), DisplayName="Link", Category="Thirdweb|Wallets|In App")
	static UAsyncTaskThirdwebLink* Link(UObject* WorldContextObject, const FInAppWalletHandle& Wallet, const FInAppWalletHandle& NewWallet, const FString& Input)
	{
		LINK_INPUT_TASK
	}

	virtual void Activate() override;
};
