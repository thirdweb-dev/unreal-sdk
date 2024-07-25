#include "ThirdwebActor.h"
#include "Engine/Engine.h"
#include <string>
#include "HttpModule.h"
#include "HttpServerModule.h"
#include "HttpServerRequest.h"
#include "HttpServerResponse.h"
#include "IHttpRouter.h"
#include "Misc/ScopeLock.h"
#include "Interfaces/IHttpResponse.h"
#include "GenericPlatform/GenericPlatformProcess.h"
#include "Misc/OutputDeviceNull.h"
#include "HAL/Event.h"
#include "Misc/ScopeLock.h"

// Sets default values
AThirdwebActor::AThirdwebActor()
{
    // Set this actor to call Tick() every frame.
    PrimaryActorTick.bCanEverTick = true;

    // Log to ensure the constructor is called
    UE_LOG(LogTemp, Warning, TEXT("AThirdwebActor constructor called"));
}

// Called when the game starts or when spawned
void AThirdwebActor::BeginPlay()
{
    Super::BeginPlay();
    UE_LOG(LogTemp, Warning, TEXT("BeginPlay called for AThirdwebActor"));
}

// Called every frame
void AThirdwebActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// Helper function to convert FFIResult to separate operation results
void ConvertFFIResultToOperationResult(const Thirdweb::FFIResult &ffi_result, bool &Success, bool &CanRetry, FString &Output)
{
    // Log FFIResult object
    UE_LOG(LogTemp, Warning, TEXT("FFIResult: success=%d, message=%s"), ffi_result.success, UTF8_TO_TCHAR(ffi_result.message));

    Success = ffi_result.success;
    Output = UTF8_TO_TCHAR(ffi_result.message);
    if (Success && Output.StartsWith("RecoverableError"))
    {
        Success = false;
        CanRetry = true;
        Output = Output.Mid(17).TrimStart();
    }
    else
    {
        CanRetry = false;
    }
    Thirdweb::free_ffi_result(ffi_result);
}

// Blueprint callable function to create a private key wallet
void AThirdwebActor::CreatePrivateKeyWallet(const FString &PrivateKey, bool &Success, bool &CanRetry, FString &Output)
{
    Thirdweb::FFIResult wallet_result = Thirdweb::create_private_key_wallet(TCHAR_TO_UTF8(*PrivateKey));
    ConvertFFIResultToOperationResult(wallet_result, Success, CanRetry, Output);
}

// Blueprint callable function to generate a private key wallet
void AThirdwebActor::GeneratePrivateKeyWallet(bool &Success, bool &CanRetry, FString &Output)
{
    Thirdweb::FFIResult wallet_result = Thirdweb::generate_private_key_wallet();
    ConvertFFIResultToOperationResult(wallet_result, Success, CanRetry, Output);
}

// Blueprint callable function to get the wallet address
void AThirdwebActor::GetWalletAddress(int64 WalletHandle, bool &Success, bool &CanRetry, FString &Output)
{
    Thirdweb::FFIResult address_result = Thirdweb::get_wallet_address(WalletHandle);
    ConvertFFIResultToOperationResult(address_result, Success, CanRetry, Output);
}

// Blueprint callable function to create an InAppWallet
void AThirdwebActor::CreateInAppWallet(const FString &Email, const FString &OAuthMethod, bool &Success, bool &CanRetry, FString &Output)
{
    const char *email = Email.IsEmpty() ? nullptr : TCHAR_TO_UTF8(*Email);
    const char *oauth_method = OAuthMethod.IsEmpty() ? nullptr : TCHAR_TO_UTF8(*OAuthMethod);
    const char *client_id = ClientID.IsEmpty() ? nullptr : TCHAR_TO_UTF8(*ClientID);
    const char *bundle_id = BundleID.IsEmpty() ? nullptr : TCHAR_TO_UTF8(*BundleID);
    const char *secret_key = SecretKey.IsEmpty() ? nullptr : TCHAR_TO_UTF8(*SecretKey);
    const char *storage_directory_path = StorageDirectoryPath.IsEmpty() ? nullptr : TCHAR_TO_UTF8(*StorageDirectoryPath);

    Thirdweb::FFIResult inapp_result = Thirdweb::create_in_app_wallet(
        client_id,
        bundle_id,
        secret_key,
        email,
        storage_directory_path,
        oauth_method);

    ConvertFFIResultToOperationResult(inapp_result, Success, CanRetry, Output);
}

// Blueprint callable function to send OTP
void AThirdwebActor::SendOTP(int64 InAppWalletHandle, bool &Success, bool &CanRetry, FString &Output)
{
    Thirdweb::FFIResult otp_result = Thirdweb::in_app_wallet_send_otp(InAppWalletHandle);
    ConvertFFIResultToOperationResult(otp_result, Success, CanRetry, Output);
}

// Blueprint callable function to verify OTP
void AThirdwebActor::VerifyOTP(int64 InAppWalletHandle, const FString &OTP, bool &Success, bool &CanRetry, FString &Output)
{
    Thirdweb::FFIResult verify_result = Thirdweb::in_app_wallet_verify_otp(InAppWalletHandle, TCHAR_TO_UTF8(*OTP));
    ConvertFFIResultToOperationResult(verify_result, Success, CanRetry, Output);
}

// Blueprint callable function to start OAuth login flow from start to finish
void AThirdwebActor::LoginWithOauthDefault(int64 InAppWalletHandle)
{
    FString RedirectUrl = TEXT("http://localhost:8789/callback");

    // Fetch the OAuth login link
    Thirdweb::FFIResult fetch_link_result = Thirdweb::in_app_wallet_fetch_oauth_login_link(InAppWalletHandle, TCHAR_TO_UTF8(*RedirectUrl));
    bool OAuthSuccess, OAuthCanRetry;
    FString OAuthOutput;
    ConvertFFIResultToOperationResult(fetch_link_result, OAuthSuccess, OAuthCanRetry, OAuthOutput);

    if (!OAuthSuccess)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to fetch OAuth login link."));
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
        UE_LOG(LogTemp, Error, TEXT("Failed to load HTTP Server module."));
        OnOAuthFailure.Broadcast(InAppWalletHandle, TEXT("Failed to load HTTP Server module."));
        return;
    }

    FHttpServerModule &HttpServerModule = FHttpServerModule::Get();
    TSharedPtr<IHttpRouter> HttpRouter = HttpServerModule.GetHttpRouter(8789);

    if (!HttpRouter.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to get HTTP Router."));
        OnOAuthFailure.Broadcast(InAppWalletHandle, TEXT("Failed to get HTTP Router."));
        return;
    }

    AuthEvent = FPlatformProcess::GetSynchEventFromPool(false);
    bAuthComplete = false;
    OAuthWalletHandle = InAppWalletHandle;

    // Bind the route
    RouteHandle = HttpRouter->BindRoute(FHttpPath(TEXT("/callback")), EHttpServerRequestVerbs::VERB_GET,
                                        [this](const FHttpServerRequest &Request, const FHttpResultCallback &OnComplete)
                                        {
                                            OAuthResult = Request.QueryParams.FindRef(TEXT("authResult"));

                                            if (OAuthResult.IsEmpty())
                                            {
                                                UE_LOG(LogTemp, Error, TEXT("AuthResult query parameter is missing."));
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
                                        });

    if (!RouteHandle.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to bind route."));
        FPlatformProcess::ReturnSynchEventToPool(AuthEvent);
        OnOAuthFailure.Broadcast(InAppWalletHandle, TEXT("Failed to bind route."));
        return;
    }

    // Start the HTTP server
    HttpServerModule.StartAllListeners();
    UE_LOG(LogTemp, Log, TEXT("HTTP Server started and listening on port 8789."));

    // Open the browser with the login URL
    FPlatformProcess::LaunchURL(*OAuthLoginUrl, nullptr, nullptr);
    UE_LOG(LogTemp, Log, TEXT("Browser opened with URL: %s"), *OAuthLoginUrl);

    // Start a timer to periodically check for the OAuth completion
    GetWorld()->GetTimerManager().SetTimerForNextTick(this, &AThirdwebActor::CheckOAuthCompletion);
}

void AThirdwebActor::CheckOAuthCompletion()
{
    if (bAuthComplete)
    {
        // Ensure the HTTP server module is loaded
        if (!FModuleManager::Get().IsModuleLoaded("HTTPServer"))
        {
            FModuleManager::Get().LoadModule("HTTPServer");
        }

        FHttpServerModule &HttpServerModule = FHttpServerModule::Get();

        // Stop the HTTP listener
        HttpServerModule.StopAllListeners();
        UE_LOG(LogTemp, Log, TEXT("HTTP Server stopped listening."));

        // Unbind the route
        TSharedPtr<IHttpRouter> HttpRouter = HttpServerModule.GetHttpRouter(8789);
        HttpRouter->UnbindRoute(RouteHandle);
        UE_LOG(LogTemp, Log, TEXT("Route unbound."));

        FPlatformProcess::ReturnSynchEventToPool(AuthEvent);

        // Set the results based on the authentication
        if (bAuthComplete && !OAuthResult.IsEmpty())
        {
            // Sign in with OAuth
            SignInWithOAuth(OAuthWalletHandle, OAuthResult, bOAuthSuccess, bOAuthCanRetry, OAuthOutputString);

            if (bOAuthSuccess)
            {
                UE_LOG(LogTemp, Log, TEXT("OAuth login flow completed successfully."));
                OnOAuthSuccess.Broadcast(OAuthWalletHandle, TEXT("Successfully signed in with OAuth."));
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("OAuth login flow failed."));
                OnOAuthFailure.Broadcast(OAuthWalletHandle, OAuthOutputString);
            }
        }
        else
        {
            bOAuthSuccess = false;
            bOAuthCanRetry = true;
            OAuthOutputString = TEXT("OAuth login flow did not complete in time.");
            UE_LOG(LogTemp, Error, TEXT("OAuth login flow did not complete in time."));
            OnOAuthFailure.Broadcast(OAuthWalletHandle, OAuthOutputString);
        }
    }
    else
    {
        // Continue checking
        GetWorld()->GetTimerManager().SetTimerForNextTick(this, &AThirdwebActor::CheckOAuthCompletion);
    }
}

// Blueprint callable function to fetch OAuth login link
void AThirdwebActor::FetchOAuthLoginLink(int64 InAppWalletHandle, const FString &RedirectUrl, bool &Success, bool &CanRetry, FString &Output)
{
    Thirdweb::FFIResult result = Thirdweb::in_app_wallet_fetch_oauth_login_link(InAppWalletHandle, TCHAR_TO_UTF8(*RedirectUrl));
    ConvertFFIResultToOperationResult(result, Success, CanRetry, Output);
}

// Blueprint callable function to sign in with OAuth
void AThirdwebActor::SignInWithOAuth(int64 InAppWalletHandle, const FString &AuthResult, bool &Success, bool &CanRetry, FString &Output)
{
    Thirdweb::FFIResult result = Thirdweb::in_app_wallet_sign_in_with_oauth(InAppWalletHandle, TCHAR_TO_UTF8(*AuthResult));
    ConvertFFIResultToOperationResult(result, Success, CanRetry, Output);
}

// Blueprint callable function to sign a message
void AThirdwebActor::SignMessage(int64 WalletHandle, const FString &Message, bool &Success, bool &CanRetry, FString &Output)
{
    Thirdweb::FFIResult sign_result = Thirdweb::sign_message(WalletHandle, TCHAR_TO_UTF8(*Message));
    ConvertFFIResultToOperationResult(sign_result, Success, CanRetry, Output);
}

// Blueprint callable function to check if a wallet is connected
void AThirdwebActor::IsConnected(int64 WalletHandle, bool &Success, bool &CanRetry, FString &Output)
{
    Thirdweb::FFIResult connected_result = Thirdweb::is_connected(WalletHandle);
    ConvertFFIResultToOperationResult(connected_result, Success, CanRetry, Output);
}

// Blueprint callable function to disconnect a wallet
void AThirdwebActor::Disconnect(int64 WalletHandle, bool &Success, bool &CanRetry, FString &Output)
{
    Thirdweb::FFIResult disconnect_result = Thirdweb::disconnect(WalletHandle);
    ConvertFFIResultToOperationResult(disconnect_result, Success, CanRetry, Output);
}

// Blueprint callable function to create a smart wallet
void AThirdwebActor::CreateSmartWallet(int64 PersonalWalletHandle,
                                       const FString &ChainID,
                                       bool Gasless,
                                       const FString &Factory,
                                       const FString &AccountOverride,
                                       bool &Success,
                                       bool &CanRetry,
                                       FString &Output)
{
    const char *client_id = ClientID.IsEmpty() ? nullptr : TCHAR_TO_UTF8(*ClientID);
    const char *bundle_id = BundleID.IsEmpty() ? nullptr : TCHAR_TO_UTF8(*BundleID);
    const char *secret_key = SecretKey.IsEmpty() ? nullptr : TCHAR_TO_UTF8(*SecretKey);
    const char *factory = Factory.IsEmpty() ? nullptr : TCHAR_TO_UTF8(*Factory);
    const char *account_override = AccountOverride.IsEmpty() ? nullptr : TCHAR_TO_UTF8(*AccountOverride);

    Thirdweb::FFIResult result = Thirdweb::create_smart_wallet(
        client_id,
        bundle_id,
        secret_key,
        PersonalWalletHandle,
        TCHAR_TO_UTF8(*ChainID),
        Gasless,
        factory,
        account_override);

    ConvertFFIResultToOperationResult(result, Success, CanRetry, Output);
}

// Blueprint callable function to check if a smart wallet is deployed
void AThirdwebActor::IsSmartWalletDeployed(int64 SmartWalletHandle, bool &Success, bool &CanRetry, FString &Output)
{
    Thirdweb::FFIResult result = Thirdweb::smart_wallet_is_deployed(SmartWalletHandle);
    ConvertFFIResultToOperationResult(result, Success, CanRetry, Output);
}

// Blueprint callable function to get all admins of a smart wallet
void AThirdwebActor::GetSmartWalletAdmins(int64 SmartWalletHandle, bool &Success, bool &CanRetry, FString &Output)
{
    Thirdweb::FFIResult result = Thirdweb::smart_wallet_get_all_admins(SmartWalletHandle);
    ConvertFFIResultToOperationResult(result, Success, CanRetry, Output);
}

// Blueprint callable function to get all active signers of a smart wallet
void AThirdwebActor::GetSmartWalletActiveSigners(int64 SmartWalletHandle, bool &Success, bool &CanRetry, FString &Output)
{
    Thirdweb::FFIResult result = Thirdweb::smart_wallet_get_all_active_signers(SmartWalletHandle);
    ConvertFFIResultToOperationResult(result, Success, CanRetry, Output);
}

// Blueprint callable function to create a session key for a smart wallet
void AThirdwebActor::CreateSmartWalletSessionKey(int64 SmartWalletHandle,
                                                 const FString &SignerAddress,
                                                 const FString &IsAdmin,
                                                 const TArray<FString> &ApprovedTargets,
                                                 const FString &NativeTokenLimitPerTransactionInWei,
                                                 const FString &PermissionStartTimestamp,
                                                 const FString &PermissionEndTimestamp,
                                                 const FString &ReqValidityStartTimestamp,
                                                 const FString &ReqValidityEndTimestamp,
                                                 bool &Success,
                                                 bool &CanRetry,
                                                 FString &Output)
{
    // Convert TArray<FString> to const char* const*
    TArray<const char *> ApprovedTargetsCArray;
    for (const FString &Target : ApprovedTargets)
    {
        ApprovedTargetsCArray.Add(TCHAR_TO_UTF8(*Target));
    }

    Thirdweb::FFIResult result = Thirdweb::smart_wallet_create_session_key(
        SmartWalletHandle,
        TCHAR_TO_UTF8(*SignerAddress),
        TCHAR_TO_UTF8(*IsAdmin),
        ApprovedTargetsCArray.GetData(),
        ApprovedTargetsCArray.Num(),
        TCHAR_TO_UTF8(*NativeTokenLimitPerTransactionInWei),
        TCHAR_TO_UTF8(*PermissionStartTimestamp),
        TCHAR_TO_UTF8(*PermissionEndTimestamp),
        TCHAR_TO_UTF8(*ReqValidityStartTimestamp),
        TCHAR_TO_UTF8(*ReqValidityEndTimestamp));

    ConvertFFIResultToOperationResult(result, Success, CanRetry, Output);
}
