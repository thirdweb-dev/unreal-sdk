// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "Wallets/ThirdwebInAppWalletHandle.h"
#include "Wallets/InApp/AsyncTaskThirdwebInAppBase.h"

#include "AsyncTaskThirdwebLoginWithOAuth.generated.h"

class UThirdwebOAuthBrowserUserWidget;

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class THIRDWEB_API UAsyncTaskThirdwebLoginWithOAuth : public UAsyncTaskThirdwebInAppBase
{
	GENERATED_BODY()

public:
	virtual void Activate() override;

	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject"), Category="Thirdweb|Wallets|In App")
	static UAsyncTaskThirdwebLoginWithOAuth* LoginWithOAuth(UObject* WorldContextObject, const FInAppWalletHandle& Wallet);

	UPROPERTY(BlueprintAssignable)
	FErrorOnlyDelegate Success;

protected:
	UPROPERTY(Transient)
	UThirdwebOAuthBrowserUserWidget* Browser;

	UPROPERTY(Transient)
	FInAppWalletHandle Wallet;

private:
	UFUNCTION()
	void HandleAuthenticated(const FString& AuthResult);

	UFUNCTION()
	virtual void HandleSignedIn();

	UFUNCTION()
	void HandleFailed(const FString& Error);
};
