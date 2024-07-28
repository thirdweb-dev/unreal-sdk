// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/AsyncTaskThirdwebLoginWithOAuth.h"

#include "HttpServerModule.h"
#include "IHttpRouter.h"
#include "ThirdwebLog.h"
#include "ThirdwebSubsystem.h"
#include "Kismet/GameplayStatics.h"


void UAsyncTaskThirdwebLoginWithOAuth::Activate()
{
	// Fetch the OAuth login URL
	FString LoginURL;
	if (FString Error; !Wallet.FetchOAuthLoginURL(TEXT("http://localhost:8789/callback"), LoginURL, Error))
	{
		return HandleFailed(Error);
	}

	// Ensure the HTTP server module is loaded
	EModuleLoadResult ModuleResult;
	FModuleManager::Get().LoadModuleWithFailureReason(FName(TEXT("HTTPServer")), ModuleResult);
	if (ModuleResult != EModuleLoadResult::Success)
	{
		return HandleFailed(TEXT("Failed to load HTTPServer"));
	}

	Router = FHttpServerModule::Get().GetHttpRouter(8789, true);
	if (!Router.IsValid())
	{
		return HandleFailed(TEXT("Failed to get HTTP Router"));
	}
	
	AuthEvent = FPlatformProcess::GetSynchEventFromPool(false);
	RouteHandle = Router->BindRoute(FHttpPath(TEXT("/callback")), EHttpServerRequestVerbs::VERB_GET, FHttpRequestHandler::CreateUObject(this, &ThisClass::CallbackRequestHandler));

	if (!RouteHandle.IsValid())
	{
		FPlatformProcess::ReturnSynchEventToPool(AuthEvent);
		return HandleFailed(TEXT("Failed to bind route"));
	}

	// Start the HTTP server
	FHttpServerModule::Get().StartAllListeners();
	TW_LOG(Verbose, TEXT("HTTP Server started and listening on port 8789"));

	// Open the browser with the login URL
	FPlatformProcess::LaunchURL(*LoginURL, nullptr, nullptr);
	TW_LOG(Log, TEXT("Browser opened with URL: %s"), *LoginURL);

	// Start a timer to periodically check for the OAuth completion
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::CheckOAuthCompletion);
}

UAsyncTaskThirdwebLoginWithOAuth* UAsyncTaskThirdwebLoginWithOAuth::LoginWithOAuth(UObject* WorldContextObject, const FWalletHandle& Wallet)
{
	if (!WorldContextObject)
	{
		return nullptr;
	}
	UAsyncTaskThirdwebLoginWithOAuth* Task = NewObject<UAsyncTaskThirdwebLoginWithOAuth>(WorldContextObject);
	Task->Wallet = Wallet;
	Task->RegisterWithGameInstance(WorldContextObject);
	return Task;
}


void UAsyncTaskThirdwebLoginWithOAuth::CheckOAuthCompletion()
{
	if (!bAuthComplete)
	{
		// Continue checking
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::CheckOAuthCompletion);
		return;
	}
	
	// Stop the HTTP listener
	FHttpServerModule::Get().StopAllListeners();
	TW_LOG(Verbose, TEXT("HTTP Server stopped listening"));

	// Unbind the route
	Router->UnbindRoute(RouteHandle);
	TW_LOG(Verbose, TEXT("Route unbound"));

	FPlatformProcess::ReturnSynchEventToPool(AuthEvent);

	// Set the results based on the authentication
	if (bAuthComplete && !OAuthResult.IsEmpty())
	{
		FString Error;
		if (Wallet.SignInWithOAuth(OAuthResult, Error))
		{
			return HandleSuccess(TEXT("Successfully signed in with OAuth."));
		}
		return HandleFailed(FString::Printf(TEXT("OAuth login flow failed: %s"), *Error));
		
	}
	HandleFailed(TEXT("OAuth login flow did not complete in time"));
}

bool UAsyncTaskThirdwebLoginWithOAuth::CallbackRequestHandler(const FHttpServerRequest& Request, const FHttpResultCallback& OnComplete)
{
	OAuthResult = Request.QueryParams.FindRef(TEXT("authResult"));

	if (OAuthResult.IsEmpty())
	{
		FString Error = TEXT("AuthResult query parameter is missing.");
		TUniquePtr<FHttpServerResponse> Response = FHttpServerResponse::Create(Error, TEXT("text/plain"));
		OnComplete(MoveTemp(Response));
		HandleFailed(Error);
	}
	else
	{
		bAuthComplete = true;
		AuthEvent->Trigger();
		TUniquePtr<FHttpServerResponse> Response = FHttpServerResponse::Create(TEXT("<html><body><h1>DONE!</h1><p>You can close this tab/window now.</p></body></html>"), TEXT("text/html"));
		OnComplete(MoveTemp(Response));
	}
	return true;
}

void UAsyncTaskThirdwebLoginWithOAuth::HandleFailed(const FString& Error)
{
	Failed.Broadcast(Error);
	SetReadyToDestroy();
}

void UAsyncTaskThirdwebLoginWithOAuth::HandleSuccess(const FString& Output)
{
	Success.Broadcast(Output);
	SetReadyToDestroy();
}
