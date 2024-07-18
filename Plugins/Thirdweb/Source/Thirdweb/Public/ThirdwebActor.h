#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Thirdweb.h"
#include <cstdint>
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
    UFUNCTION(BlueprintCallable, Category = "Thirdweb")
    FString CreatePrivateKeyWallet(const FString &PrivateKey);

    // Blueprint callable function to generate a private key wallet
    UFUNCTION(BlueprintCallable, Category = "Thirdweb")
    FString GeneratePrivateKeyWallet();

    // Blueprint callable function to get the wallet address
    UFUNCTION(BlueprintCallable, Category = "Thirdweb")
    FString GetWalletAddress(int64 WalletHandle);

    // Blueprint callable function to create an InAppWallet
    UFUNCTION(BlueprintCallable, Category = "Thirdweb")
    FString CreateInAppWallet(const FString &Email);

    // Blueprint callable function to send OTP
    UFUNCTION(BlueprintCallable, Category = "Thirdweb")
    FString SendOTP(int64 InAppWalletHandle);

    // Blueprint callable function to verify OTP
    UFUNCTION(BlueprintCallable, Category = "Thirdweb")
    FString VerifyOTP(int64 InAppWalletHandle, const FString &OTP);

    // Blueprint callable function to sign a message
    UFUNCTION(BlueprintCallable, Category = "Thirdweb")
    FString SignMessage(int64 WalletHandle, const FString &Message);

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
