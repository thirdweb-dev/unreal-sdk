// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "HttpRouteHandle.h"
#include "ThirdwebWalletHandle.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "AsyncTaskThirdwebLoginWithOAuth.generated.h"

class IHttpRouter;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOAuthDelegate, const FString&, Message);

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

	UPROPERTY(BlueprintAssignable)
	FOAuthDelegate Success;

	UPROPERTY(BlueprintAssignable)
	FOAuthDelegate Failed;

protected:
	UPROPERTY(Transient)
	FWalletHandle Wallet;

	UPROPERTY(Transient)
	bool bAuthComplete;

	UPROPERTY(Transient)
	FString OAuthResult;

	FEvent* AuthEvent;
	FHttpRouteHandle RouteHandle;
	TSharedPtr<IHttpRouter> Router;
	
private:
	void CheckOAuthCompletion();
	bool CallbackRequestHandler(const FHttpServerRequest& Request, const FHttpResultCallback& OnComplete);
	
	void HandleFailed(const FString& Error);
	void HandleSuccess(const FString& Output);
};
