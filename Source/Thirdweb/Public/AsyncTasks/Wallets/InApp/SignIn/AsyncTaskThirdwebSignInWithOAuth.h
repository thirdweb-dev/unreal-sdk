// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "AsyncTaskThirdwebSignInBase.h"
#include "AsyncTaskThirdwebSignInWithOAuth.generated.h"

UCLASS(Blueprintable, BlueprintType)
class THIRDWEB_API UAsyncTaskThirdwebSignInWithOAuth : public UAsyncTaskThirdwebSignInBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject"), DisplayName="Sign In With OAuth", Category="Thirdweb|Wallets|In App")
	static UAsyncTaskThirdwebSignInWithOAuth* SignInWithOAuth(UObject* WorldContextObject, const FInAppWalletHandle& Wallet, UPARAM(DisplayName="Auth Result") const FString& Input)
	{
		SIGN_IN_INPUT_TASK
	}
	
	virtual void Activate() override;
};
