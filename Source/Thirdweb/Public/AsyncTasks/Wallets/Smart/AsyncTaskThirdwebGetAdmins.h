// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "AsyncTaskThirdwebSmartBase.h"
#include "AsyncTaskThirdwebGetAdmins.generated.h"

/**
 * Get all admins of a smart wallet
 */
UCLASS(Blueprintable, BlueprintType)
class THIRDWEB_API UAsyncTaskThirdwebGetAdmins : public UAsyncTaskThirdwebSmartBase
{
	GENERATED_BODY()

public:
	virtual void Activate() override;

	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject"), Category="Thirdweb|Wallets|Smart")
	static UAsyncTaskThirdwebGetAdmins* GetAdmins(UObject* WorldContextObject, const FSmartWalletHandle& Wallet);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGetAdminsDelegate, const TArray<FString>&, Admins, const FString&, Error);
	UPROPERTY(BlueprintAssignable)
	FGetAdminsDelegate Success;

	UPROPERTY(BlueprintAssignable)
	FGetAdminsDelegate Failed;
private:
	UFUNCTION()
	void HandleResponse(const TArray<FString>& Admins);
	
	UFUNCTION()
	virtual void HandleFailed(const FString& Error);
};
