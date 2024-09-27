// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "HttpRouteHandle.h"

#include "UObject/Object.h"

#include "ThirdwebOAuthExternalBrowser.generated.h"

class IHttpRouter;

/**
 * 
 */
UCLASS()
class THIRDWEB_API UThirdwebOAuthExternalBrowser : public UObject, public FTickableGameObject
{
	GENERATED_BODY()

public:
	UThirdwebOAuthExternalBrowser();
	
	void Authenticate(const FString& Link);
	
	/** FTickableGameObject implementation */
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override { return TStatId(); };
	
private:
	bool CallbackRequestHandler(const FHttpServerRequest& Request, const FHttpResultCallback& OnComplete);
	void HandleSuccess();
	void HandleError(const FString& Error);

public:
	DECLARE_DELEGATE_OneParam(FSimpleStringDelegate, const FString&);
	FSimpleStringDelegate OnAuthenticated;
	FSimpleStringDelegate OnError;

private:
	enum EState
	{
		Initialized,
		AuthPending,
		AuthComplete,
		Complete
	};
	
	FString AuthResult;
	FEvent* AuthEvent;
	FHttpRouteHandle RouteHandle;
	TSharedPtr<IHttpRouter> Router;
	EState State;
	
};
