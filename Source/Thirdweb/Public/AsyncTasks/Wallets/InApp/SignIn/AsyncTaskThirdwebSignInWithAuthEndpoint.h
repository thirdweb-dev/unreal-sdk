// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "AsyncTaskThirdwebSignInBase.h"
#include "AsyncTaskThirdwebSignInWithAuthEndpoint.generated.h"

UCLASS(Abstract)
class THIRDWEB_API UAsyncTaskThirdwebSignInWithAuthEndpoint : public UAsyncTaskThirdwebSignInBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject"), DisplayName="Sign In With Auth Endpoint", Category="Thirdweb|Wallets|In App")
	static UAsyncTaskThirdwebSignInWithAuthEndpoint* SignInWithAuthEndpoint(UObject* WorldContextObject, const FInAppWalletHandle& Wallet, UPARAM(DisplayName="Payload") const FString& Input)
	{
		SIGN_IN_INPUT_TASK
	}
	
	virtual void Activate() override;
};
