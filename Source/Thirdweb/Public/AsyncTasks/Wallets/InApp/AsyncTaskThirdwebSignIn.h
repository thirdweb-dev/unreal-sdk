// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "AsyncTasks/Wallets/InApp/AsyncTaskThirdwebInAppBase.h"
#include "AsyncTaskThirdwebSignIn.generated.h"

UCLASS(Abstract)
class THIRDWEB_API UAsyncTaskThirdwebSignIn : public UAsyncTaskThirdwebInAppBase
{
	GENERATED_BODY()

public:
	/** Multi-Purpose runtime sign-in node covering all sign in options based upon Wallet. Input is ignored if guest wallet */
	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject"), DisplayName="Sign In", Category="Thirdweb|Wallets|In App")
	static UAsyncTaskThirdwebSignIn* SignIn(UObject* WorldContextObject, const FInAppWalletHandle& Wallet, const FString& Input, const FString& Signature);

	UPROPERTY(BlueprintAssignable)
	FErrorOnlyDelegate Success;

	UPROPERTY(BlueprintAssignable)
	FErrorOnlyDelegate Failed;
	
protected:
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
