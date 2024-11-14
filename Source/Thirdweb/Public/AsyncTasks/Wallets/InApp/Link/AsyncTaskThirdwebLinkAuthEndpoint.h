// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "AsyncTaskThirdwebLinkBase.h"
#include "AsyncTaskThirdwebLinkAuthEndpoint.generated.h"

UCLASS(Abstract)
class THIRDWEB_API UAsyncTaskThirdwebLinkAuthEndpoint : public UAsyncTaskThirdwebLinkBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject"), DisplayName="Link Auth Endpoint", Category="Thirdweb|Wallets|In App")
	static UAsyncTaskThirdwebLinkAuthEndpoint* LinkAuthEndpoint(UObject* WorldContextObject,
	                                                            const FInAppWalletHandle& Wallet,
	                                                            const FInAppWalletHandle& NewWallet,
	                                                            UPARAM(DisplayName="Payload") const FString& Input)
	{
		LINK_INPUT_TASK
	}

	virtual void Activate() override;
};
