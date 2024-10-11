// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "AsyncTaskThirdwebInAppCreateWalletBase.h"
#include "ThirdwebRuntimeSettings.h"
#include "AsyncTaskThirdwebCreateOAuthWallet.generated.h"

UCLASS(Blueprintable, BlueprintType)
class THIRDWEB_API UAsyncTaskThirdwebCreateOAuthWallet : public UAsyncTaskThirdwebInAppCreateWalletBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject", AdvancedDisplay="PartnerId"), Category="Thirdweb|Wallets|InApp")
	static UAsyncTaskThirdwebCreateOAuthWallet* CreateOAuthWallet(UObject* WorldContextObject, const EThirdwebOAuthProvider Provider, const FString& PartnerId);

protected:
	UPROPERTY(Transient)
	EThirdwebOAuthProvider Provider;
	
public:
	virtual void Activate() override;
};
