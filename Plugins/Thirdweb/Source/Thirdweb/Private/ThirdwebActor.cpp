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
    Thirdweb::FFIResult inapp_result = Thirdweb::create_in_app_wallet(TCHAR_TO_UTF8(*ClientID), TCHAR_TO_UTF8(*BundleID), TCHAR_TO_UTF8(*SecretKey), TCHAR_TO_UTF8(*Email), TCHAR_TO_UTF8(*StorageDirectoryPath));
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
