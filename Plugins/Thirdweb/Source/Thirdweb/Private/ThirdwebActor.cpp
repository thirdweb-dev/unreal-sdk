#include "ThirdwebActor.h"
#include "Engine/Engine.h"
#include <string>

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
void AThirdwebActor::CreateInAppWallet(const FString &Email, bool &Success, bool &CanRetry, FString &Output)
{
    const char *client_id = ClientID.IsEmpty() ? nullptr : TCHAR_TO_UTF8(*ClientID);
    const char *bundle_id = BundleID.IsEmpty() ? nullptr : TCHAR_TO_UTF8(*BundleID);
    const char *secret_key = SecretKey.IsEmpty() ? nullptr : TCHAR_TO_UTF8(*SecretKey);
    const char *storage_directory_path = StorageDirectoryPath.IsEmpty() ? nullptr : TCHAR_TO_UTF8(*StorageDirectoryPath);

    Thirdweb::FFIResult inapp_result = Thirdweb::create_in_app_wallet(
        client_id,
        bundle_id,
        secret_key,
        TCHAR_TO_UTF8(*Email),
        storage_directory_path);

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
