// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "AsyncTasks/Wallets/InApp/AsyncTaskThirdwebInAppBase.h"
#include "AsyncTaskThirdwebLink.generated.h"

UCLASS(Abstract)
class THIRDWEB_API UAsyncTaskThirdwebLink : public UAsyncTaskThirdwebInAppBase
{
	GENERATED_BODY()

public:
	/** Multi-Purpose runtime link node covering all link options based upon Wallet. Input is ignored if guest wallet */
	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject"), DisplayName="Link", Category="Thirdweb|Wallets|In App")
	static UAsyncTaskThirdwebLink* Link(UObject* WorldContextObject, const FInAppWalletHandle& Wallet, const FInAppWalletHandle& NewWallet, const FString& Input, const FString& Signature);

	UPROPERTY(BlueprintAssignable)
	FErrorOnlyDelegate Success;

	UPROPERTY(BlueprintAssignable)
	FErrorOnlyDelegate Failed;
	
protected:
	UPROPERTY(Transient)
	FInAppWalletHandle NewInAppWallet;
	
	UPROPERTY(Transient)
	FString AuthInput;

	UPROPERTY(Transient)
	FString SignatureInput;

public:
	virtual void Activate() override;

protected:
	UFUNCTION()
	void HandleResponse();

	UFUNCTION()
	virtual void HandleFailed(const FString& Error);
};
