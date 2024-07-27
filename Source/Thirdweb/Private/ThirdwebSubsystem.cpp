// Copyright (c) 2024 Thirdweb. All Rights Reserved.

// ReSharper disable CppUE4BlueprintCallableFunctionMayBeStatic

#include "ThirdwebSubsystem.h"
#include "HttpServerModule.h"
#include "IHttpRouter.h"
#include "Thirdweb.h"
#include "ThirdwebLog.h"
#include "Kismet/GameplayStatics.h"

void UThirdwebSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	TW_LOG(Log, TEXT("ThirdwebSubsystem::Intitialize::Subsystem initialization complete"))
}

void UThirdwebSubsystem::Deinitialize()
{
	Super::Deinitialize();
	TW_LOG(Log, TEXT("ThirdwebSubsystem::Deinitialize::Subsystem deinitialization complete"))
}

void UThirdwebSubsystem::CreatePrivateKeyWallet(const FString& PrivateKey, bool& Success, bool& CanRetry, FString& Output)
{
	Thirdweb::create_private_key_wallet(StringCast<ANSICHAR>(*PrivateKey).Get()).ToOperationResult(Success, CanRetry, Output);
}

void UThirdwebSubsystem::GeneratePrivateKeyWallet(bool& Success, bool& CanRetry, FString& Output)
{
	Thirdweb::generate_private_key_wallet().ToOperationResult(Success, CanRetry, Output);
}

void UThirdwebSubsystem::GetWalletAddress(const int64 WalletHandle, bool& Success, bool& CanRetry, FString& Output)
{
	Thirdweb::get_wallet_address(WalletHandle).ToOperationResult(Success, CanRetry, Output);
}

void UThirdwebSubsystem::CreateInAppWallet(const FString& Email, const FString& OAuthMethod, bool& bSuccess, bool& CanRetry, FString& Output)
{
	Thirdweb::create_in_app_wallet(
		StringCast<ANSICHAR>(*BundleID).Get(),
		StringCast<ANSICHAR>(*Email).Get(),
		StringCast<ANSICHAR>(*SecretKey).Get(),
		StringCast<ANSICHAR>(*Email).Get(),
		StringCast<ANSICHAR>(*StorageDirectoryPath).Get(),
		StringCast<ANSICHAR>(*OAuthMethod).Get()
	).ToOperationResult(bSuccess, CanRetry, Output);
}

void UThirdwebSubsystem::SendOTP(int64 InAppWalletHandle, bool& Success, bool& CanRetry, FString& Output)
{
	Thirdweb::in_app_wallet_send_otp(InAppWalletHandle).ToOperationResult(Success, CanRetry, Output);
}

void UThirdwebSubsystem::VerifyOTP(int64 InAppWalletHandle, const FString& OTP, bool& Success, bool& CanRetry, FString& Output)
{
	Thirdweb::in_app_wallet_verify_otp(InAppWalletHandle, StringCast<ANSICHAR>(*OTP).Get()).ToOperationResult(Success, CanRetry, Output);
}

void UThirdwebSubsystem::LoginWithOauthDefault(int64 InAppWalletHandle)
{
	FString RedirectUrl = TEXT("http://localhost:8789/callback");
	bool OAuthSuccess, OAuthCanRetry;
	FString OAuthOutput;
	Thirdweb::in_app_wallet_fetch_oauth_login_link(InAppWalletHandle, StringCast<ANSICHAR>(*RedirectUrl).Get())
		.ToOperationResult(OAuthSuccess, OAuthCanRetry, OAuthOutput);

	if (!OAuthSuccess)
	{
		TW_LOG(Error, TEXT("Failed to fetch OAuth login link."));
		OnOAuthFailure.Broadcast(InAppWalletHandle, OAuthOutput);
		return;
	}

	OAuthLoginUrl = OAuthOutput;

	// Ensure the HTTP server module is loaded
	if (!FModuleManager::Get().IsModuleLoaded("HTTPServer"))
	{
		FModuleManager::Get().LoadModule("HTTPServer");
	}

	if (!FModuleManager::Get().IsModuleLoaded("HTTPServer"))
	{
		TW_LOG(Error, TEXT("Failed to load HTTP Server module."));
		OnOAuthFailure.Broadcast(InAppWalletHandle, TEXT("Failed to load HTTP Server module."));
		return;
	}

	FHttpServerModule& HttpServerModule = FHttpServerModule::Get();
	TSharedPtr<IHttpRouter> HttpRouter = HttpServerModule.GetHttpRouter(8789);

	if (!HttpRouter.IsValid())
	{
		TW_LOG(Error, TEXT("Failed to get HTTP Router."));
		OnOAuthFailure.Broadcast(InAppWalletHandle, TEXT("Failed to get HTTP Router."));
		return;
	}

	AuthEvent = FPlatformProcess::GetSynchEventFromPool(false);
	bAuthComplete = false;
	OAuthWalletHandle = InAppWalletHandle;

	// Bind the route
	RouteHandle = HttpRouter->BindRoute(
		FHttpPath(TEXT("/callback")),
		EHttpServerRequestVerbs::VERB_GET,
		FHttpRequestHandler::CreateLambda([this](const FHttpServerRequest& Request, const FHttpResultCallback& OnComplete) -> bool
		{
			OAuthResult = Request.QueryParams.FindRef(TEXT("authResult"));

			if (OAuthResult.IsEmpty())
			{
				TW_LOG(Error, TEXT("AuthResult query parameter is missing."));
				TUniquePtr<FHttpServerResponse> Response = FHttpServerResponse::Create(TEXT("AuthResult query parameter is missing."), TEXT("text/plain"));
				OnComplete(MoveTemp(Response));
				OnOAuthFailure.Broadcast(OAuthWalletHandle, TEXT("AuthResult query parameter is missing."));
				return true;
			}

			bAuthComplete = true;
			AuthEvent->Trigger();

			// Respond to the browser
			TUniquePtr<FHttpServerResponse> Response = FHttpServerResponse::Create(TEXT("<html><body><h1>DONE!</h1><p>You can close this tab/window now.</p></body></html>"), TEXT("text/html"));
			OnComplete(MoveTemp(Response));

			return true;
		}));

	if (!RouteHandle.IsValid())
	{
		TW_LOG(Error, TEXT("Failed to bind route."));
		FPlatformProcess::ReturnSynchEventToPool(AuthEvent);
		OnOAuthFailure.Broadcast(InAppWalletHandle, TEXT("Failed to bind route."));
		return;
	}

	// Start the HTTP server
	HttpServerModule.StartAllListeners();
	TW_LOG(Log, TEXT("HTTP Server started and listening on port 8789."));

	// Open the browser with the login URL
	FPlatformProcess::LaunchURL(*OAuthLoginUrl, nullptr, nullptr);
	TW_LOG(Log, TEXT("Browser opened with URL: %s"), *OAuthLoginUrl);

	// Start a timer to periodically check for the OAuth completion
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UThirdwebSubsystem::CheckOAuthCompletion);
}

void UThirdwebSubsystem::CheckOAuthCompletion()
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

		FPlatformProcess::ReturnSynchEventToPool(AuthEvent);

		// Set the results based on the authentication
		if (bAuthComplete && !OAuthResult.IsEmpty())
		{
			// Sign in with OAuth
			SignInWithOAuth(OAuthWalletHandle, OAuthResult, bOAuthSuccess, bOAuthCanRetry, OAuthOutputString);

			if (bOAuthSuccess)
			{
				TW_LOG(Log, TEXT("OAuth login flow completed successfully."));
				OnOAuthSuccess.Broadcast(OAuthWalletHandle, TEXT("Successfully signed in with OAuth."));
			}
			else
			{
				TW_LOG(Error, TEXT("OAuth login flow failed."));
				OnOAuthFailure.Broadcast(OAuthWalletHandle, OAuthOutputString);
			}
		}
		else
		{
			bOAuthSuccess = false;
			bOAuthCanRetry = true;
			OAuthOutputString = TEXT("OAuth login flow did not complete in time.");
			TW_LOG(Error, TEXT("OAuth login flow did not complete in time."));
			OnOAuthFailure.Broadcast(OAuthWalletHandle, OAuthOutputString);
		}
	}
	else
	{
		// Continue checking
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UThirdwebSubsystem::CheckOAuthCompletion);
	}
}

void UThirdwebSubsystem::FetchOAuthLoginLink(int64 InAppWalletHandle, const FString& RedirectUrl, bool& Success, bool& CanRetry, FString& Output)
{
	Thirdweb::in_app_wallet_fetch_oauth_login_link(InAppWalletHandle, StringCast<ANSICHAR>(*RedirectUrl).Get()).ToOperationResult(Success, CanRetry, Output);
}

void UThirdwebSubsystem::SignInWithOAuth(int64 InAppWalletHandle, const FString& AuthResult, bool& Success, bool& CanRetry, FString& Output)
{
	Thirdweb::in_app_wallet_sign_in_with_oauth(InAppWalletHandle, StringCast<ANSICHAR>(*AuthResult).Get()).ToOperationResult(Success, CanRetry, Output);
}

void UThirdwebSubsystem::SignMessage(int64 WalletHandle, const FString& Message, bool& Success, bool& CanRetry, FString& Output)
{
	Thirdweb::sign_message(WalletHandle, StringCast<ANSICHAR>(*Message).Get()).ToOperationResult(Success, CanRetry, Output);
}

void UThirdwebSubsystem::IsConnected(int64 WalletHandle, bool& Success, bool& CanRetry, FString& Output)
{
	Thirdweb::is_connected(WalletHandle).ToOperationResult(Success, CanRetry, Output);
}

void UThirdwebSubsystem::Disconnect(int64 WalletHandle, bool& Success, bool& CanRetry, FString& Output)
{
	Thirdweb::disconnect(WalletHandle).ToOperationResult(Success, CanRetry, Output);
}

UThirdwebSubsystem* UThirdwebSubsystem::Get(const UObject* WorldContextObject)
{
	if (UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject))
	{
		return GameInstance->GetSubsystem<UThirdwebSubsystem>();
	}
	return nullptr;
}

void UThirdwebSubsystem::CreateSmartWallet(int64 PersonalWalletHandle,
                                           const FString& ChainID,
                                           bool Gasless,
                                           const FString& Factory,
                                           const FString& AccountOverride,
                                           bool& Success,
                                           bool& CanRetry,
                                           FString& Output)
{
	Thirdweb::create_smart_wallet(
		StringCast<ANSICHAR>(*ClientID).Get(),
		StringCast<ANSICHAR>(*BundleID).Get(),
		StringCast<ANSICHAR>(*SecretKey).Get(),
		PersonalWalletHandle,
		StringCast<ANSICHAR>(*ChainID).Get(),
		Gasless,
		StringCast<ANSICHAR>(*Factory).Get(),
		StringCast<ANSICHAR>(*AccountOverride).Get()
	).ToOperationResult(Success, CanRetry, Output);
}

void UThirdwebSubsystem::IsSmartWalletDeployed(int64 SmartWalletHandle, bool& Success, bool& CanRetry, FString& Output)
{
	Thirdweb::smart_wallet_is_deployed(SmartWalletHandle).ToOperationResult(Success, CanRetry, Output);
}

void UThirdwebSubsystem::GetSmartWalletAdmins(int64 SmartWalletHandle, bool& Success, bool& CanRetry, FString& Output)
{
	Thirdweb::smart_wallet_get_all_admins(SmartWalletHandle).ToOperationResult(Success, CanRetry, Output);
}

// Blueprint callable function to get all active signers of a smart wallet
void UThirdwebSubsystem::GetSmartWalletActiveSigners(int64 SmartWalletHandle, bool& Success, bool& CanRetry, FString& Output)
{
	Thirdweb::smart_wallet_get_all_active_signers(SmartWalletHandle).ToOperationResult(Success, CanRetry, Output);
}

// Blueprint callable function to create a session key for a smart wallet
void UThirdwebSubsystem::CreateSmartWalletSessionKey(int64 SmartWalletHandle,
                                                     const FString& SignerAddress,
                                                     const FString& IsAdmin,
                                                     const TArray<FString>& ApprovedTargets,
                                                     const FString& NativeTokenLimitPerTransactionInWei,
                                                     const FString& PermissionStartTimestamp,
                                                     const FString& PermissionEndTimestamp,
                                                     const FString& ReqValidityStartTimestamp,
                                                     const FString& ReqValidityEndTimestamp,
                                                     bool& Success,
                                                     bool& CanRetry,
                                                     FString& Output)
{
	// Convert TArray<FString> to const char* const*
	TArray<const char*> ApprovedTargetsCArray;
	for (const FString& Target : ApprovedTargets)
	{
		ApprovedTargetsCArray.Add(StringCast<ANSICHAR>(*Target).Get());
	}

	Thirdweb::smart_wallet_create_session_key(
		SmartWalletHandle,
		StringCast<ANSICHAR>(*SignerAddress).Get(),
		StringCast<ANSICHAR>(*IsAdmin).Get(),
		ApprovedTargetsCArray.GetData(),
		ApprovedTargetsCArray.Num(),
		StringCast<ANSICHAR>(*NativeTokenLimitPerTransactionInWei).Get(),
		StringCast<ANSICHAR>(*PermissionStartTimestamp).Get(),
		StringCast<ANSICHAR>(*PermissionEndTimestamp).Get(),
		StringCast<ANSICHAR>(*ReqValidityStartTimestamp).Get(),
		StringCast<ANSICHAR>(*ReqValidityEndTimestamp).Get()
	).ToOperationResult(Success, CanRetry, Output);
}
