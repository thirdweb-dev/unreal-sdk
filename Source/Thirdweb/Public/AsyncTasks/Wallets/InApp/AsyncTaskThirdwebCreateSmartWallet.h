// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "AsyncTasks/AsyncTaskThirdwebBase.h"

#include "Wallets/ThirdwebInAppWalletHandle.h"

#include "AsyncTaskThirdwebCreateSmartWallet.generated.h"

struct FSmartWalletHandle;

UCLASS(Blueprintable, BlueprintType)
class THIRDWEB_API UAsyncTaskThirdwebCreateSmartWallet : public UAsyncTaskThirdwebBase
{
	GENERATED_BODY()

public:
	virtual void Activate() override;

	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject"), Category="Thirdweb|Wallets|Smart")
	static UAsyncTaskThirdwebCreateSmartWallet* CreateSmartWallet(UObject* WorldContextObject,
	                                                              const FInAppWalletHandle& InAppWallet,
	                                                              const int64 ChainID,
	                                                              const bool bGasless = true,
	                                                              const FString& Factory = "",
	                                                              const FString& AccountOverride = "");

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCreateSmartWalletDelegate, const FSmartWalletHandle&, SmartWallet, const FString&, Error);

	UPROPERTY(BlueprintAssignable)
	FCreateSmartWalletDelegate Success;

	UPROPERTY(BlueprintAssignable)
	FCreateSmartWalletDelegate Failed;

protected:
	UPROPERTY(Transient)
	FInAppWalletHandle InAppWallet;

	UPROPERTY(Transient)
	int64 ChainID;

	UPROPERTY(Transient)
	bool bGasless;

	UPROPERTY(Transient)
	FString Factory;

	UPROPERTY(Transient)
	FString AccountOverride;

private:
	UFUNCTION()
	void HandleResponse(const FSmartWalletHandle& Wallet);

	UFUNCTION()
	virtual void HandleFailed(const FString& Error);
};
