// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "AsyncTaskThirdwebInAppBase.h"
#include "AsyncTaskThirdwebSignInWithOTP.generated.h"

/**
 * Verifies an OTP for the in-app wallet
 */
UCLASS(Blueprintable, BlueprintType)
class THIRDWEB_API UAsyncTaskThirdwebSignInWithOTP : public UAsyncTaskThirdwebInAppBase
{
	GENERATED_BODY()

public:
	virtual void Activate() override;

	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject"), DisplayName="Sign In With OTP", Category="Thirdweb|Wallets|In App")
	static UAsyncTaskThirdwebSignInWithOTP* SignInWithOTP(UObject* WorldContextObject, const FInAppWalletHandle& Wallet, const FString& OTP);
	
	UPROPERTY(BlueprintAssignable)
	FErrorOnlyDelegate Success;

	UPROPERTY(BlueprintAssignable)
	FErrorOnlyDelegate Failed;
	
protected:
	UPROPERTY(Transient)
	FString OTP;
	
private:
	UFUNCTION()
	void HandleResponse();
	
	UFUNCTION()
	virtual void HandleFailed(const FString& Error);
};
