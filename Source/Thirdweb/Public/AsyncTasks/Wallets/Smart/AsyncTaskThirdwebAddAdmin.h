// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "AsyncTaskThirdwebSmartBase.h"
#include "AsyncTaskThirdwebAddAdmin.generated.h"

/**
 * Revoke the session key of a smart wallet signer
 */
UCLASS(Blueprintable, BlueprintType)
class THIRDWEB_API UAsyncTaskThirdwebAddAdmin : public UAsyncTaskThirdwebSmartBase
{
	GENERATED_BODY()

public:
	virtual void Activate() override;

	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject"), Category="Thirdweb|Wallets|Smart")
	static UAsyncTaskThirdwebAddAdmin* AddAdmin(UObject* WorldContextObject, const FSmartWalletHandle& Wallet, const FString& Signer);
	
	UPROPERTY(BlueprintAssignable)
	FErrorOnlyDelegate Success;

	UPROPERTY(BlueprintAssignable)
	FErrorOnlyDelegate Failed;
	
protected:
	UPROPERTY(Transient)
	FString Signer;
	
private:
	UFUNCTION()
	void HandleResponse();
	
	UFUNCTION()
	virtual void HandleFailed(const FString& Error);
};
