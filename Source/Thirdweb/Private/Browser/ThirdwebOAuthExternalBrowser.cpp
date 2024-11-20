// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "Browser/ThirdwebOAuthExternalBrowser.h"

#include "HttpRequestHandler.h"
#include "HttpServerModule.h"
#include "IHttpRouter.h"
#include "ThirdwebLog.h"
#include "Delegates/DelegateSignatureImpl.inl"
#include "GenericPlatform/GenericPlatformHttp.h"
#include "HAL/Event.h"
#include "Misc/EngineVersionComparison.h"

UThirdwebOAuthExternalBrowser::UThirdwebOAuthExternalBrowser()
{
	AuthEvent = nullptr;
	State = Initialized;
	bIsSiwe = false;
}

void UThirdwebOAuthExternalBrowser::Authenticate(const FString& Link)
{
	EModuleLoadResult ModuleResult;
	FModuleManager::Get().LoadModuleWithFailureReason(FName(TEXT("HTTPServer")), ModuleResult);
	if (ModuleResult != EModuleLoadResult::Success)
	{
		return HandleError(TEXT("Failed to load HTTPServer"));
	}

	Router = FHttpServerModule::Get().GetHttpRouter(8789, true);
	if (!Router.IsValid())
	{
		return HandleError(TEXT("Failed to get HTTP Router"));
	}
	
	AuthEvent = FPlatformProcess::GetSynchEventFromPool(false);

#if UE_VERSION_OLDER_THAN(5, 4, 0)
	FHttpRequestHandler Handler = [WeakThis = MakeWeakObjectPtr(this)](const FHttpServerRequest& Request, const FHttpResultCallback& OnComplete)
	{
		if (WeakThis.IsValid())
		{
			return WeakThis->OAuthCallbackRequestHandler(Request, OnComplete);
		}
		return false;
	};
#else
	FHttpRequestHandler Handler = FHttpRequestHandler::CreateUObject(this, &UThirdwebOAuthExternalBrowser::OAuthCallbackRequestHandler);
#endif
	
	RouteHandle = Router->BindRoute(FHttpPath(TEXT("/callback")), EHttpServerRequestVerbs::VERB_GET, Handler);

	if (!RouteHandle.IsValid())
	{
		FPlatformProcess::ReturnSynchEventToPool(AuthEvent);
		return HandleError(TEXT("Failed to bind route"));
	}

	// Start the HTTP server
	FHttpServerModule::Get().StartAllListeners();
	TW_LOG(VeryVerbose, TEXT("OAuth HTTP Server started and listening on port 8789"));

	// Open the browser with the login URL
	FPlatformProcess::LaunchURL(*Link, nullptr, nullptr);
	TW_LOG(Verbose, TEXT("Browser opened with URL: %s"), *Link);
	State = AuthPending;
}

void UThirdwebOAuthExternalBrowser::SignInWithEthereum()
{
	EModuleLoadResult ModuleResult;
	FModuleManager::Get().LoadModuleWithFailureReason(FName(TEXT("HTTPServer")), ModuleResult);
	if (ModuleResult != EModuleLoadResult::Success)
	{
		return HandleError(TEXT("Failed to load HTTPServer"));
	}

	Router = FHttpServerModule::Get().GetHttpRouter(8789, true);
	if (!Router.IsValid())
	{
		return HandleError(TEXT("Failed to get HTTP Router"));
	}
	
	AuthEvent = FPlatformProcess::GetSynchEventFromPool(false);

#if UE_VERSION_OLDER_THAN(5, 4, 0)
	FHttpRequestHandler Handler = [WeakThis = MakeWeakObjectPtr(this)](const FHttpServerRequest& Request, const FHttpResultCallback& OnComplete)
	{
		if (WeakThis.IsValid())
		{
			return WeakThis->SiweCallbackRequestHandler(Request, OnComplete);
		}
		return false;
	};
#else
	FHttpRequestHandler Handler = FHttpRequestHandler::CreateUObject(this, &UThirdwebOAuthExternalBrowser::SiweCallbackRequestHandler);
#endif
	
	RouteHandle = Router->BindRoute(FHttpPath(TEXT("/callback")), EHttpServerRequestVerbs::VERB_GET, Handler);

	if (!RouteHandle.IsValid())
	{
		FPlatformProcess::ReturnSynchEventToPool(AuthEvent);
		return HandleError(TEXT("Failed to bind route"));
	}

	// Start the HTTP server
	FHttpServerModule::Get().StartAllListeners();
	TW_LOG(VeryVerbose, TEXT("OAuth HTTP Server started and listening on port 8789"));
	
	bIsSiwe = true;
	
	// Open the browser with the login URL
	const FString RedirectUrl = FGenericPlatformHttp::UrlEncode(TEXT("http://localhost:8789/callback"));
	FPlatformProcess::LaunchURL(*FString::Printf(TEXT("http://static.thirdweb.com/auth/siwe?redirectUrl=%s"), *RedirectUrl), nullptr, nullptr);
	TW_LOG(Verbose, TEXT("Browser opened with URL: %s"), TEXT("http://localhost:8789"));
	State = AuthPending;
}

void UThirdwebOAuthExternalBrowser::Tick(float DeltaTime)
{
	if (State == AuthComplete)
	{
		State = Complete;

		// Set the results based on the authentication
		if (bIsSiwe)
		{
			if (!Signature.IsEmpty() && !Payload.IsEmpty())
			{
				return HandleSuccess();
			}
		} else
		{
			if (!AuthResult.IsEmpty())
			{
				return HandleSuccess();
			}
		}
		
		HandleError(TEXT("OAuth login flow did not complete in time"));
	}
}

void UThirdwebOAuthExternalBrowser::BeginDestroy()
{
	// Stop the HTTP listener
	FHttpServerModule::Get().StopAllListeners();
	TW_LOG(VeryVerbose, TEXT("OAuth HTTP Server stopped listening"));

	if (Router.IsValid() && RouteHandle.IsValid())
	{
		// Unbind the route
		Router->UnbindRoute(RouteHandle);
		TW_LOG(VeryVerbose, TEXT("Route unbound"));
	}
	if (AuthEvent)
	{
		FPlatformProcess::ReturnSynchEventToPool(AuthEvent);
	}
	ConditionalBeginDestroy();
	
	UObject::BeginDestroy();
}

bool UThirdwebOAuthExternalBrowser::OAuthCallbackRequestHandler(const FHttpServerRequest& Request, const FHttpResultCallback& OnComplete)
{
	AuthResult = Request.QueryParams.FindRef(TEXT("authResult"));
	if (AuthResult.IsEmpty())
	{
		FString Error = TEXT("AuthResult query parameter is missing.");
		TUniquePtr<FHttpServerResponse> Response = FHttpServerResponse::Create(Error, TEXT("text/plain"));
		OnComplete(MoveTemp(Response));
		HandleError(Error);
	}
	else
	{
		State = AuthComplete;
		AuthEvent->Trigger();
		TUniquePtr<FHttpServerResponse> Response = FHttpServerResponse::Create(
			TEXT("<script>window.location.replace('https://static.thirdweb.com/auth/complete')</script>"), TEXT("text/html")
		);
		OnComplete(MoveTemp(Response));
	}
	return true;
}

bool UThirdwebOAuthExternalBrowser::SiweCallbackRequestHandler(const FHttpServerRequest& Request, const FHttpResultCallback& OnComplete)
{
	Signature = Request.QueryParams.FindRef(TEXT("signature"));
	Payload = FGenericPlatformHttp::UrlDecode(Request.QueryParams.FindRef(TEXT("payload")));
	UE_LOG(LogTemp, VeryVerbose, TEXT("UThirdwebOAuthExternalBrowser::SiweCallbackRequestHandler::Signature=%s|Payload=%s"), *Signature, *Payload)
	if (Signature.IsEmpty() || Payload.IsEmpty())
	{
		FString Error = TEXT("Signature/Payload query parameter is missing.");
		TUniquePtr<FHttpServerResponse> Response = FHttpServerResponse::Create(Error, TEXT("text/plain"));
		OnComplete(MoveTemp(Response));
		HandleError(Error);
	}
	else
	{
		State = AuthComplete;
		AuthEvent->Trigger();
		TUniquePtr<FHttpServerResponse> Response = FHttpServerResponse::Create(
			TEXT("<script>window.location.replace('https://static.thirdweb.com/auth/complete')</script>"), TEXT("text/html")
		);
		OnComplete(MoveTemp(Response));
	}
	return true;
}

void UThirdwebOAuthExternalBrowser::HandleSuccess()
{
	if (bIsSiwe)
	{
		if (OnSiweComplete.IsBound())
		{
			OnSiweComplete.Execute(Signature, Payload);
		}
	} else
	{
		if (OnAuthenticated.IsBound())
		{
			OnAuthenticated.Execute(AuthResult);
		}
	}
	
}

void UThirdwebOAuthExternalBrowser::HandleError(const FString& Error)
{
	
	TW_LOG(VeryVerbose, TEXT("UThirdwebOAuthExternalBrowser::HandleError::%s"), *Error);
	if (OnError.IsBound())
	{
		OnError.Execute(Error);
	}
}
