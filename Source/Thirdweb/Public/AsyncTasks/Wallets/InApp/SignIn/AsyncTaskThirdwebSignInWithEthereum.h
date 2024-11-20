// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "AsyncTaskThirdwebSignInBase.h"
#include "AsyncTaskThirdwebSignInWithEthereum.generated.h"

UCLASS(Abstract)
class THIRDWEB_API UAsyncTaskThirdwebSignInWithEthereum : public UAsyncTaskThirdwebSignInBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject", Keywords="SIWE"), DisplayName="Sign In With Ethereum", Category="Thirdweb|Wallets|In App")
	static UAsyncTaskThirdwebSignInWithEthereum* SignInWithAuthEndpoint(UObject* WorldContextObject,
	                                                                    const FInAppWalletHandle& Wallet,
	                                                                    UPARAM(DisplayName="Payload") const FString& Input,
	                                                                    const FString& Signature)
	{
		NEW_TASK
		Task->InAppWallet = Wallet;
		Task->AuthInput = Input;
		Task->SignatureInput = Signature;
		RR_TASK
	}

protected:
	UPROPERTY(Transient)
	FString SignatureInput;

public:
	virtual void Activate() override;
};
