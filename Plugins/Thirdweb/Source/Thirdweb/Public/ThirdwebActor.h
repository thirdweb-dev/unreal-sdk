#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Thirdweb.h"
#include "ThirdwebActor.generated.h"

UCLASS()
class THIRDWEB_API AThirdwebActor : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AThirdwebActor();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Blueprint callable function to create a private key wallet
    // Thirdweb -> Private Key category
    UFUNCTION(BlueprintCallable, Category = "Thirdweb|PrivateKeyWallet")
    void CreatePrivateKeyWallet(const FString &PrivateKey, bool &Success, bool &CanRetry, FString &Output);

    // Blueprint callable function to generate a private key wallet
    UFUNCTION(BlueprintCallable, Category = "Thirdweb|PrivateKeyWallet")
    void GeneratePrivateKeyWallet(bool &Success, bool &CanRetry, FString &Output);

    // Blueprint callable function to create an InAppWallet
    UFUNCTION(BlueprintCallable, Category = "Thirdweb|InAppWallet")
    void CreateInAppWallet(const FString &Email, bool &Success, bool &CanRetry, FString &Output);

    // Blueprint callable function to send OTP
    UFUNCTION(BlueprintCallable, Category = "Thirdweb|InAppWallet")
    void SendOTP(int64 InAppWalletHandle, bool &Success, bool &CanRetry, FString &Output);

    // Blueprint callable function to verify OTP
    UFUNCTION(BlueprintCallable, Category = "Thirdweb|InAppWallet")
    void VerifyOTP(int64 InAppWalletHandle, const FString &OTP, bool &Success, bool &CanRetry, FString &Output);

    // Blueprint callable function to get the wallet address
    UFUNCTION(BlueprintCallable, Category = "Thirdweb|Wallets")
    void GetWalletAddress(int64 WalletHandle, bool &Success, bool &CanRetry, FString &Output);

    // Blueprint callable function to sign a message
    UFUNCTION(BlueprintCallable, Category = "Thirdweb|Wallets")
    void SignMessage(int64 WalletHandle, const FString &Message, bool &Success, bool &CanRetry, FString &Output);

    // Blueprint callable function to check if a wallet is connected
    UFUNCTION(BlueprintCallable, Category = "Thirdweb|Wallets")
    void IsConnected(int64 WalletHandle, bool &Success, bool &CanRetry, FString &Output);

    // Blueprint callable function to disconnect a wallet
    UFUNCTION(BlueprintCallable, Category = "Thirdweb|Wallets")
    void Disconnect(int64 WalletHandle, bool &Success, bool &CanRetry, FString &Output);

    // Properties for configurable parameters
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thirdweb")
    FString ClientID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thirdweb")
    FString BundleID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thirdweb")
    FString SecretKey;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thirdweb")
    FString StorageDirectoryPath;
};
