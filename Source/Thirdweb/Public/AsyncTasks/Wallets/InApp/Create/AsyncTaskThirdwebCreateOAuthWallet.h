// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "AsyncTaskThirdwebInAppCreateWalletBase.h"
#include "AsyncTaskThirdwebCreateOAuthWallet.generated.h"

enum class EThirdwebOAuthProvider : uint8;

UCLASS(Blueprintable, BlueprintType)
class THIRDWEB_API UAsyncTaskThirdwebCreateOAuthWallet : public UAsyncTaskThirdwebInAppCreateWalletBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject"), Category="Thirdweb|Wallets|InApp")
	static UAsyncTaskThirdwebCreateOAuthWallet* CreateOAuthWallet(UObject* WorldContextObject, const EThirdwebOAuthProvider Provider);

protected:
	UPROPERTY(Transient)
	EThirdwebOAuthProvider Provider;
	
public:
	virtual void Activate() override;
};
