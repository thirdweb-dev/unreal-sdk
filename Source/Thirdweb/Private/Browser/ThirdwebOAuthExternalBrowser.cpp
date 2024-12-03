// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "Browser/ThirdwebOAuthExternalBrowser.h"

#include "HttpRequestHandler.h"
#include "HttpServerModule.h"
#include "IHttpRouter.h"
#include "ThirdwebLog.h"
#include "ThirdwebRuntimeSettings.h"
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
	FString Url = Link;
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
			return WeakThis->CallbackRequestHandler(Request, OnComplete);
		}
		return false;
	};
#else
	FHttpRequestHandler Handler = FHttpRequestHandler::CreateUObject(this, &UThirdwebOAuthExternalBrowser::CallbackRequestHandler);
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

	bIsSiwe = Url.ToUpper().TrimStartAndEnd().Equals(TEXT("SIWE"));
	if (bIsSiwe)
	{
		Url = FString::Printf(TEXT("http://static.thirdweb.com/auth/siwe?redirectUrl=%s"), *FGenericPlatformHttp::UrlEncode(TEXT("http://localhost:8789/callback")));
	}
	// Open the browser with the login URL
	FPlatformProcess::LaunchURL(*Link, nullptr, nullptr);
	TW_LOG(Verbose, TEXT("Browser opened with URL: %s"), *Link);
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

bool UThirdwebOAuthExternalBrowser::CallbackRequestHandler(const FHttpServerRequest& Request, const FHttpResultCallback& OnComplete)
{
	AuthResult = Request.QueryParams.FindRef(TEXT("authResult"));
	Signature = Request.QueryParams.FindRef(TEXT("signature"));
	Payload = FGenericPlatformHttp::UrlDecode(Request.QueryParams.FindRef(TEXT("payload")));
	TW_LOG(VeryVerbose, TEXT("UThirdwebOAuthExternalBrowser::CallbackRequestHandler::Signature=%s|Payload=%s|AuthResult=%s"), *Signature, *Payload, *AuthResult)
	if (bIsSiwe ? Signature.IsEmpty() || Payload.IsEmpty() : AuthResult.IsEmpty())
	{
		FString Error = FString::Printf(TEXT("%s query parameter is missing"), bIsSiwe ? TEXT("Signature/Payload") : TEXT("AuthResult"));
		TUniquePtr<FHttpServerResponse> Response = FHttpServerResponse::Create(Error, TEXT("text/plain"));
		OnComplete(MoveTemp(Response));
		HandleError(Error);
	}
	else
	{
		State = AuthComplete;
		AuthEvent->Trigger();
		TUniquePtr<FHttpServerResponse> Response = FHttpServerResponse::Create(
			FString::Printf(TEXT("<script>window.location.replace('%s')</script>"), *UThirdwebRuntimeSettings::GetExternalAuthRedirectUri()),
			TEXT("text/html")
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



