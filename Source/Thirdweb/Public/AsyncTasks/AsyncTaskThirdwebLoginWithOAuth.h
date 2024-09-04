// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "ThirdwebWalletHandle.h"

#include "Kismet/BlueprintAsyncActionBase.h"

#include "AsyncTaskThirdwebLoginWithOAuth.generated.h"

class UThirdwebOAuthBrowserUserWidget;

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, ClassGroup="Thirdweb|Authentication")
class THIRDWEB_API UAsyncTaskThirdwebLoginWithOAuth : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	
	virtual void Activate() override;

	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject"), Category="Thirdweb|Wallets|In App")
	static UAsyncTaskThirdwebLoginWithOAuth* LoginWithOAuth(UObject* WorldContextObject, const FWalletHandle& Wallet);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOAuthDelegate, const FString&, Message);
	UPROPERTY(BlueprintAssignable)
	FOAuthDelegate Success;

	UPROPERTY(BlueprintAssignable)
	FOAuthDelegate Failed;

protected:
	UPROPERTY(Transient)
	UThirdwebOAuthBrowserUserWidget* Browser;
	
	UPROPERTY(Transient)
	FWalletHandle Wallet;
	
private:
	UFUNCTION()
	void HandleFailed(const FString& Error);

	UFUNCTION()
	void HandleSuccess();
};
