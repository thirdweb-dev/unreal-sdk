// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "AsyncTaskThirdwebInAppBase.h"
#include "AsyncTaskThirdwebGetLinkedAccounts.generated.h"

UCLASS(Blueprintable, BlueprintType)
class THIRDWEB_API UAsyncTaskThirdwebGetLinkedAccounts : public UAsyncTaskThirdwebInAppBase
{
	GENERATED_BODY()

public:
	virtual void Activate() override;

	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject"), Category="Thirdweb|Wallets|In App")
	static UAsyncTaskThirdwebGetLinkedAccounts* GetLinkedAccounts(UObject* WorldContextObject, const FInAppWalletHandle& Wallet);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGetLinkedAccountsDelegate, const TArray<FThirdwebLinkedAccount>&, LinkedAccounts, const FString&, Error);
	UPROPERTY(BlueprintAssignable)
	FGetLinkedAccountsDelegate Success;
	
	UPROPERTY(BlueprintAssignable)
	FGetLinkedAccountsDelegate Failed;

private:
	UFUNCTION()
	void HandleResponse(const TArray<FThirdwebLinkedAccount>& LinkedAccounts);
	
	UFUNCTION()
	virtual void HandleFailed(const FString& Error);
};
