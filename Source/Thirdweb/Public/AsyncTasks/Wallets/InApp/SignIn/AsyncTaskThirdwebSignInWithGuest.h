// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "AsyncTaskThirdwebSignInBase.h"
#include "AsyncTaskThirdwebSignInWithGuest.generated.h"

UCLASS(Blueprintable, BlueprintType)
class THIRDWEB_API UAsyncTaskThirdwebSignInWithGuest : public UAsyncTaskThirdwebSignInBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject"), DisplayName="Sign In With Guest", Category="Thirdweb|Wallets|In App")
	static UAsyncTaskThirdwebSignInWithGuest* SignInWithGuest(UObject* WorldContextObject, const FInAppWalletHandle& Wallet) { SIGN_IN_TASK }

	virtual void Activate() override;
};
