// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "HttpRouteHandle.h"
#include "Tickable.h"
#include "UObject/Object.h"
#include "ThirdwebOAuthExternalBrowser.generated.h"

class IHttpRouter;

/**
 * 
 */
UCLASS(NotBlueprintable, NotBlueprintType, MinimalAPI)
class UThirdwebOAuthExternalBrowser : public UObject, public FTickableGameObject
{
	GENERATED_BODY()

public:
	UThirdwebOAuthExternalBrowser();
	
	void Authenticate(const FString& Link);
	void SignInWithEthereum();
	
	/** FTickableGameObject implementation */
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override { return TStatId(); };
	virtual void BeginDestroy() override;
	
private:
	bool OAuthCallbackRequestHandler(const FHttpServerRequest& Request, const FHttpResultCallback& OnComplete);
	bool SiweCallbackRequestHandler(const FHttpServerRequest& Request, const FHttpResultCallback& OnComplete);
	void HandleSuccess();
	void HandleError(const FString& Error);

public:
	DECLARE_DELEGATE_OneParam(FSimpleStringDelegate, const FString&);
	FSimpleStringDelegate OnAuthenticated;
	FSimpleStringDelegate OnError;
	
	DECLARE_DELEGATE_TwoParams(FDoubleStringDelegate, const FString&, const FString&);
	FDoubleStringDelegate OnSiweComplete;
private:
	enum EState
	{
		Initialized,
		AuthPending,
		AuthComplete,
		Complete
	};

	// OAuth
	FString AuthResult;
	// SIWE
	FString Signature;
	FString Payload;
	bool bIsSiwe;
	
	FEvent* AuthEvent;
	FHttpRouteHandle RouteHandle;
	TSharedPtr<IHttpRouter> Router;
	EState State;
};
