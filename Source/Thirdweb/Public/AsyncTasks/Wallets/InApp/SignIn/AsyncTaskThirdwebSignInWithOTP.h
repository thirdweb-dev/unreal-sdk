// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "AsyncTaskThirdwebSignInBase.h"
#include "AsyncTaskThirdwebSignInWithOTP.generated.h"

UCLASS(Abstract)
class THIRDWEB_API UAsyncTaskThirdwebSignInWithOTP : public UAsyncTaskThirdwebSignInBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject"), DisplayName="Sign In With OTP", Category="Thirdweb|Wallets|In App")
	static UAsyncTaskThirdwebSignInWithOTP* SignInWithOTP(UObject* WorldContextObject, const FInAppWalletHandle& Wallet, UPARAM(DisplayName="OTP") const FString& Input)
	{
		SIGN_IN_INPUT_TASK
	}
	
	virtual void Activate() override;
};
