// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "AsyncTaskThirdwebInAppBase.h"
#include "AsyncTaskThirdwebSendOTP.generated.h"

/**
 * Sends an OTP for the in-app wallet
 */
UCLASS(Blueprintable, BlueprintType)
class THIRDWEB_API UAsyncTaskThirdwebSendOTP : public UAsyncTaskThirdwebInAppBase
{
	GENERATED_BODY()

public:
	virtual void Activate() override;

	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject"), DisplayName="Send OTP", Category="Thirdweb|Wallets|In App")
	static UAsyncTaskThirdwebSendOTP* SendOTP(UObject* WorldContextObject, const FInAppWalletHandle& Wallet);
	
	UPROPERTY(BlueprintAssignable)
	FErrorOnlyDelegate Success;
	
	UPROPERTY(BlueprintAssignable)
	FErrorOnlyDelegate Failed;

private:
	UFUNCTION()
	void HandleResponse();
	
	UFUNCTION()
	virtual void HandleFailed(const FString& Error);
};
