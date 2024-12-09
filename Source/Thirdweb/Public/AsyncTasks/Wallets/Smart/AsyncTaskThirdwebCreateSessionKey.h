// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "AsyncTaskThirdwebSmartBase.h"
#include "Misc/DateTime.h"
#include "AsyncTaskThirdwebCreateSessionKey.generated.h"

/**
 * Create a session key for a smart wallet
 */
UCLASS(Blueprintable, BlueprintType)
class THIRDWEB_API UAsyncTaskThirdwebCreateSessionKey : public UAsyncTaskThirdwebSmartBase
{
	GENERATED_BODY()

public:
	virtual void Activate() override;

	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject", AdvancedDisplay="PermissionStart,PermissionEnd,RequestValidityStart,RequestValidityEnd",
		AutoCreateRefTerm="PermissionEnd,ApprovedTargets,Signer,NativeTokenLimitPerTransactionInWei"), Category="Thirdweb|Wallets|Smart")
	static UAsyncTaskThirdwebCreateSessionKey* CreateSessionKey(UObject* WorldContextObject,
	                                                            const FSmartWalletHandle& Wallet,
	                                                            const FString& Signer,
	                                                            const TArray<FString>& ApprovedTargets,
	                                                            const FString& NativeTokenLimitPerTransactionInWei,
	                                                            const FDateTime& PermissionEnd);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCreateSessionKeyDelegate, const FString&, TxHash, const FString&, Error);
	UPROPERTY(BlueprintAssignable)
	FCreateSessionKeyDelegate Success;
	
	UPROPERTY(BlueprintAssignable)
	FCreateSessionKeyDelegate Failed;
	
protected:
	UPROPERTY(Transient)
	FString Signer;

	UPROPERTY(Transient)
	TArray<FString> ApprovedTargets;

	UPROPERTY(Transient)
	FString NativeTokenLimitPerTransactionInWei;

	UPROPERTY(Transient)
	FDateTime PermissionEnd;

private:
	UFUNCTION()
	void HandleResponse(const FString& TxHash);
	
	UFUNCTION()
	virtual void HandleFailed(const FString& Error);
};
