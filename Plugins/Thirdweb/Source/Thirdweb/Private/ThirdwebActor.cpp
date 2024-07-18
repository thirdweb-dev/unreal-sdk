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

// Blueprint callable function to create a private key wallet
FString AThirdwebActor::CreatePrivateKeyWallet(const FString &PrivateKey)
{
    Thirdweb::FFIResult wallet_result = Thirdweb::create_private_key_wallet(TCHAR_TO_UTF8(*PrivateKey));
    if (wallet_result.success)
    {
        int64 WalletHandle = std::stoll(wallet_result.message);
        FString WalletHandleStr(UTF8_TO_TCHAR(wallet_result.message));
        Thirdweb::free_ffi_result(wallet_result);
        return WalletHandleStr;
    }
    else
    {
        FString ErrorStr(UTF8_TO_TCHAR(wallet_result.message));
        Thirdweb::free_ffi_result(wallet_result);
        return ErrorStr;
    }
}

// Blueprint callable function to generate a private key wallet
FString AThirdwebActor::GeneratePrivateKeyWallet()
{
    Thirdweb::FFIResult wallet_result = Thirdweb::generate_private_key_wallet();
    if (wallet_result.success)
    {
        int64 WalletHandle = std::stoll(wallet_result.message);
        FString WalletHandleStr(UTF8_TO_TCHAR(wallet_result.message));
        Thirdweb::free_ffi_result(wallet_result);
        return WalletHandleStr;
    }
    else
    {
        FString ErrorStr(UTF8_TO_TCHAR(wallet_result.message));
        Thirdweb::free_ffi_result(wallet_result);
        return ErrorStr;
    }
}

// Blueprint callable function to get the wallet address
FString AThirdwebActor::GetWalletAddress(int64 WalletHandle)
{
    Thirdweb::FFIResult address_result = Thirdweb::get_wallet_address(WalletHandle);
    if (address_result.success)
    {
        FString AddressStr(UTF8_TO_TCHAR(address_result.message));
        Thirdweb::free_ffi_result(address_result);
        return AddressStr;
    }
    else
    {
        FString ErrorStr(UTF8_TO_TCHAR(address_result.message));
        Thirdweb::free_ffi_result(address_result);
        return ErrorStr;
    }
}

// Blueprint callable function to create an InAppWallet
FString AThirdwebActor::CreateInAppWallet(const FString &Email)
{
    Thirdweb::FFIResult inapp_result = Thirdweb::create_in_app_wallet(TCHAR_TO_UTF8(*ClientID), TCHAR_TO_UTF8(*BundleID), TCHAR_TO_UTF8(*SecretKey), TCHAR_TO_UTF8(*Email), TCHAR_TO_UTF8(*StorageDirectoryPath));
    if (inapp_result.success)
    {
        int64 InAppWalletHandle = std::stoll(inapp_result.message);
        FString InAppWalletHandleStr(UTF8_TO_TCHAR(inapp_result.message));
        Thirdweb::free_ffi_result(inapp_result);
        return InAppWalletHandleStr;
    }
    else
    {
        FString ErrorStr(UTF8_TO_TCHAR(inapp_result.message));
        Thirdweb::free_ffi_result(inapp_result);
        return ErrorStr;
    }
}

// Blueprint callable function to send OTP
FString AThirdwebActor::SendOTP(int64 InAppWalletHandle)
{
    Thirdweb::FFIResult otp_result = Thirdweb::in_app_wallet_send_otp(InAppWalletHandle);
    if (otp_result.success)
    {
        FString OtpStr(UTF8_TO_TCHAR(otp_result.message));
        Thirdweb::free_ffi_result(otp_result);
        return OtpStr;
    }
    else
    {
        FString ErrorStr(UTF8_TO_TCHAR(otp_result.message));
        Thirdweb::free_ffi_result(otp_result);
        return ErrorStr;
    }
}

// Blueprint callable function to verify OTP
FString AThirdwebActor::VerifyOTP(int64 InAppWalletHandle, const FString &OTP)
{
    Thirdweb::FFIResult verify_result = Thirdweb::in_app_wallet_verify_otp(InAppWalletHandle, TCHAR_TO_UTF8(*OTP));
    if (verify_result.success)
    {
        FString VerifyStr(UTF8_TO_TCHAR(verify_result.message));
        Thirdweb::free_ffi_result(verify_result);
        return VerifyStr;
    }
    else
    {
        FString ErrorStr(UTF8_TO_TCHAR(verify_result.message));
        Thirdweb::free_ffi_result(verify_result);
        return ErrorStr;
    }
}

// Blueprint callable function to sign a message
FString AThirdwebActor::SignMessage(int64 WalletHandle, const FString &Message)
{
    Thirdweb::FFIResult sign_result = Thirdweb::sign_message(WalletHandle, TCHAR_TO_UTF8(*Message));
    if (sign_result.success)
    {
        FString SignatureStr(UTF8_TO_TCHAR(sign_result.message));
        Thirdweb::free_ffi_result(sign_result);
        return SignatureStr;
    }
    else
    {
        FString ErrorStr(UTF8_TO_TCHAR(sign_result.message));
        Thirdweb::free_ffi_result(sign_result);
        return ErrorStr;
    }
}
