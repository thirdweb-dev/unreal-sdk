// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "HttpRouteHandle.h"
#include "ThirdwebWalletHandle.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "AsyncTaskThirdwebLoginWithOAuth.generated.h"

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
	
	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly="true", DefaultToSelf="Widget"), Category=UI)
	static UAsyncTaskThirdwebLoginWithOAuth* LoginWithOAuth(UObject* WorldContextObject, const FWalletHandle& Wallet);
	
	UPROPERTY(BlueprintAssignable)
	FOAuthDelegate Success;

	UPROPERTY(BlueprintAssignable)
	FOAuthDelegate Failed;

protected:
	UPROPERTY(Transient)
	UThirdwebSubsystem* Subsystem;
	
	UPROPERTY(Transient)
	FWalletHandle Wallet;

	UPROPERTY(Transient)
	bool bAuthComplete;

	UPROPERTY(Transient)
	FString OAuthResult;
	
	FEvent* Event;
	FHttpRouteHandle RouteHandle;
private:
	void Authenticate( FString OAuthLoginURL);
	void CheckOAuthCompletion();
};
