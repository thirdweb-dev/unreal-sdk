// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "AsyncTaskThirdwebSmartBase.h"
#include "AsyncTaskThirdwebIsDeployed.generated.h"

/**
 * Check if a smart wallet is deployed
 */
UCLASS(Blueprintable, BlueprintType)
class THIRDWEB_API UAsyncTaskThirdwebIsDeployed : public UAsyncTaskThirdwebSmartBase
{
	GENERATED_BODY()

public:
	virtual void Activate() override;

	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject"), Category="Thirdweb|Wallets|Smart")
	static UAsyncTaskThirdwebIsDeployed* IsDeployed(UObject* WorldContextObject, const FSmartWalletHandle& Wallet);
	
	UPROPERTY(BlueprintAssignable)
	FErrorOnlyDelegate Deployed;
	
	UPROPERTY(BlueprintAssignable)
	FErrorOnlyDelegate NotDeployed;

	UPROPERTY(BlueprintAssignable)
	FErrorOnlyDelegate Failed;

private:
	UFUNCTION()
	void HandleResponse(const bool bIsDeployed);
	
	UFUNCTION()
	virtual void HandleFailed(const FString& Error);
};
