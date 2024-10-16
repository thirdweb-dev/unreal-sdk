// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "AsyncTaskThirdwebSmartBase.h"
#include "AsyncTaskThirdwebIsActiveSigner.generated.h"

/**
 * Checks whether the provided backend wallet is currently an active signer or not
 */
UCLASS(Blueprintable, BlueprintType)
class THIRDWEB_API UAsyncTaskThirdwebIsActiveSigner : public UAsyncTaskThirdwebSmartBase
{
	GENERATED_BODY()

public:
	virtual void Activate() override;

	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject"), Category="Thirdweb|Wallets|Smart")
	static UAsyncTaskThirdwebIsActiveSigner* IsActiveSigner(UObject* WorldContextObject, const FSmartWalletHandle& Wallet, const FString& BackendWallet);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FIsActiveSignerDelegate, const bool, bActiveSigner, const FString&, Error);
	UPROPERTY(BlueprintAssignable)
	FIsActiveSignerDelegate Success;

	UPROPERTY(BlueprintAssignable)
	FIsActiveSignerDelegate Failed;
	
protected:
	UPROPERTY(Transient)
	FString BackendWallet;
	
private:
	UFUNCTION()
	void HandleIsDeployedResponse(const bool& bDeployed);
	
	UFUNCTION()
	void HandleResponse(const TArray<FSigner>& Signers);
	
	UFUNCTION()
	virtual void HandleFailed(const FString& Error);
};
