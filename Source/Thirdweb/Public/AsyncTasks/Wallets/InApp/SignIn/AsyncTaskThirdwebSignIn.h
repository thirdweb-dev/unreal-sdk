// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "AsyncTaskThirdwebSignInBase.h"
#include "AsyncTaskThirdwebSignIn.generated.h"

UCLASS(Abstract)
class THIRDWEB_API UAsyncTaskThirdwebSignIn : public UAsyncTaskThirdwebSignInBase
{
	GENERATED_BODY()

public:
	/** Multi-Purpose runtime sign-in node covering all sign in options based upon Wallet. Input is ignored if guest wallet */
	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject"), DisplayName="Sign In", Category="Thirdweb|Wallets|In App")
	static UAsyncTaskThirdwebSignIn* SignIn(UObject* WorldContextObject, const FInAppWalletHandle& Wallet, const FString& Input)
	{
		SIGN_IN_INPUT_TASK
	}
	
	virtual void Activate() override;
};
