// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "AsyncTaskThirdwebInAppBase.h"
#include "AsyncTaskThirdwebVerifyOTP.generated.h"

/**
 * Verifies an OTP for the in-app wallet
 */
UCLASS(Blueprintable, BlueprintType)
class THIRDWEB_API UAsyncTaskThirdwebVerifyOTP : public UAsyncTaskThirdwebInAppBase
{
	GENERATED_BODY()

public:
	virtual void Activate() override;

	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject"), DisplayName="Verify OTP", Category="Thirdweb|Wallets|In App")
	static UAsyncTaskThirdwebVerifyOTP* VerifyOTP(UObject* WorldContextObject, const FInAppWalletHandle& Wallet, const FString& OTP);
	
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
