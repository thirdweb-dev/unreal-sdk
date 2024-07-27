// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/AsyncTaskThirdwebLoginWithOAuth.h"

#include "HttpServerModule.h"
#include "IHttpRouter.h"
#include "ThirdwebLog.h"
#include "ThirdwebSubsystem.h"
#include "Kismet/GameplayStatics.h"

class FHttpServerModule;

void UAsyncTaskThirdwebLoginWithOAuth::Activate()
{
	if (UThirdwebSubsystem* ThirdwebSubsystem = UThirdwebSubsystem::Get(GetWorld()))
	{
		//ThirdwebSubsystem->LoginWithOAuthDefault(Wallet, FOauthResponseDelegate::BindUObject(this, &ThisClass::Authenticate), FOauthResponseDelegate::CreateWeakLambda(this, [&](FString Msg)
		//{
			
		//}));
	}
}

UAsyncTaskThirdwebLoginWithOAuth* UAsyncTaskThirdwebLoginWithOAuth::LoginWithOAuth(UObject* WorldContextObject, const FWalletHandle& Wallet)
{
	if (!WorldContextObject)
	{
		return nullptr;
	}
	UAsyncTaskThirdwebLoginWithOAuth* Task = NewObject<UAsyncTaskThirdwebLoginWithOAuth>(WorldContextObject);
	Task->Wallet = Wallet;
	Task->Subsystem = UThirdwebSubsystem::Get(WorldContextObject);
	Task->RegisterWithGameInstance(WorldContextObject);
	return Task;
}

void UAsyncTaskThirdwebLoginWithOAuth::Authenticate(FString OAuthLoginURL)
{
	// Ensure the HTTP server module is loaded
	if (!FModuleManager::Get().IsModuleLoaded("HTTPServer"))
	{
		FModuleManager::Get().LoadModule("HTTPServer");
	}

	if (!FModuleManager::Get().IsModuleLoaded("HTTPServer"))
	{
		TW_LOG(Error, TEXT("Failed to load HTTP Server module."));
		Failed.Broadcast(TEXT("Failed to load HTTP Server module."));
		return;
	}

	FHttpServerModule& HttpServerModule = FHttpServerModule::Get();
	TSharedPtr<IHttpRouter> HttpRouter = HttpServerModule.GetHttpRouter(8789);

	if (!HttpRouter.IsValid())
	{
		TW_LOG(Error, TEXT("Failed to get HTTP Router."));
		Failed.Broadcast(TEXT("Failed to get HTTP Router."));
		
		return;
	}

	Event = FPlatformProcess::GetSynchEventFromPool(false);
	bAuthComplete = false;

	// Bind the route
	RouteHandle = HttpRouter->BindRoute(
		FHttpPath(TEXT("/callback")),
		EHttpServerRequestVerbs::VERB_GET,
		FHttpRequestHandler::CreateWeakLambda(this, [&](const FHttpServerRequest& Request, const FHttpResultCallback& OnComplete) -> bool
		{
			OAuthResult = Request.QueryParams.FindRef(TEXT("authResult"));

			if (OAuthResult.IsEmpty())
			{
				TW_LOG(Error, TEXT("AuthResult query parameter is missing."));
				TUniquePtr<FHttpServerResponse> Response = FHttpServerResponse::Create(TEXT("AuthResult query parameter is missing."), TEXT("text/plain"));
				OnComplete(MoveTemp(Response));
				Failed.Broadcast(TEXT("AuthResult query parameter is missing."));
				return true;
			}

			bAuthComplete = true;
			Event->Trigger();

			// Respond to the browser
			TUniquePtr<FHttpServerResponse> Response = FHttpServerResponse::Create(TEXT("<html><body><h1>DONE!</h1><p>You can close this tab/window now.</p></body></html>"), TEXT("text/html"));
			OnComplete(MoveTemp(Response));

			return true;
		}));

	if (!RouteHandle.IsValid())
	{
		TW_LOG(Error, TEXT("Failed to bind route."));
		FPlatformProcess::ReturnSynchEventToPool(Event);
		Failed.Broadcast(TEXT("Failed to bind route."));
		return;
	}

	// Start the HTTP server
	HttpServerModule.StartAllListeners();
	TW_LOG(Log, TEXT("HTTP Server started and listening on port 8789."));

	// Open the browser with the login URL
	FPlatformProcess::LaunchURL(*OAuthLoginURL, nullptr, nullptr);
	TW_LOG(Log, TEXT("Browser opened with URL: %s"), *OAuthLoginURL);

	// Start a timer to periodically check for the OAuth completion
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::CheckOAuthCompletion);
}

void UAsyncTaskThirdwebLoginWithOAuth::CheckOAuthCompletion()
{
	if (bAuthComplete)
	{
		// Ensure the HTTP server module is loaded
		if (!FModuleManager::Get().IsModuleLoaded("HTTPServer"))
		{
			FModuleManager::Get().LoadModule("HTTPServer");
		}

		FHttpServerModule& HttpServerModule = FHttpServerModule::Get();

		// Stop the HTTP listener
		HttpServerModule.StopAllListeners();
		TW_LOG(Log, TEXT("HTTP Server stopped listening."));

		// Unbind the route
		TSharedPtr<IHttpRouter> HttpRouter = HttpServerModule.GetHttpRouter(8789);
		HttpRouter->UnbindRoute(RouteHandle);
		TW_LOG(Log, TEXT("Route unbound."));

		FPlatformProcess::ReturnSynchEventToPool(Event);

		// Set the results based on the authentication
		if (bAuthComplete && !OAuthResult.IsEmpty())
		{
			bool bCanRetry;
			FString Output;
			// Sign in with OAuth
			if (Subsystem->SignInWithOAuth(Wallet, OAuthResult, bCanRetry, Output))
			{
				TW_LOG(Log, TEXT("OAuth login flow completed successfully."));
				Success.Broadcast(TEXT("Successfully signed in with OAuth."));
			}
			else
			{
				TW_LOG(Error, TEXT("OAuth login flow failed."));
				Failed.Broadcast(Output);
			}
		}
		else
		{
			TW_LOG(Error, TEXT("OAuth login flow did not complete in time."));
			Failed.Broadcast(TEXT("OAuth login flow did not complete in time."));
		}
	}
	else
	{
		// Continue checking
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::CheckOAuthCompletion);
	}
}

