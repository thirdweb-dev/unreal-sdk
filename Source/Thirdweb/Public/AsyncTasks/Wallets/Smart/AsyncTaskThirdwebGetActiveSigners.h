// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "AsyncTaskThirdwebSmartBase.h"
#include "AsyncTaskThirdwebGetActiveSigners.generated.h"

/**
 * Get all active signers of a smart wallet
 */
UCLASS(Blueprintable, BlueprintType)
class THIRDWEB_API UAsyncTaskThirdwebGetActiveSigners : public UAsyncTaskThirdwebSmartBase
{
	GENERATED_BODY()

public:
	virtual void Activate() override;

	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject"), Category="Thirdweb|Wallets|Smart")
	static UAsyncTaskThirdwebGetActiveSigners* GetActiveSigners(UObject* WorldContextObject, const FSmartWalletHandle& Wallet);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGetActiveSignersDelegate, const TArray<FSigner>&, ActiveSigners, const FString&, Error);
	UPROPERTY(BlueprintAssignable)
	FGetActiveSignersDelegate Success;
	
	UPROPERTY(BlueprintAssignable)
	FGetActiveSignersDelegate Failed;
	
private:
	UFUNCTION()
	void HandleIsDeployedResponse(const bool bDeployed);
	
	UFUNCTION()
	void HandleResponse(const TArray<FSigner>& Signers);
	
	UFUNCTION()
	virtual void HandleFailed(const FString& Error);
};
