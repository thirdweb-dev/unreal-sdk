// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "AsyncTaskThirdwebSignInBase.h"
#include "AsyncTaskThirdwebSignInWithJwt.generated.h"

UCLASS(Abstract)
class THIRDWEB_API UAsyncTaskThirdwebSignInWithJwt : public UAsyncTaskThirdwebSignInBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject"), DisplayName="Sign In With JWT", Category="Thirdweb|Wallets|InApp")
	static UAsyncTaskThirdwebSignInWithJwt* SignInWithJwt(UObject* WorldContextObject, const FInAppWalletHandle& Wallet, UPARAM(DisplayName="JWT") const FString& Input)
	{
		SIGN_IN_INPUT_TASK
	}
	
	virtual void Activate() override;
};
