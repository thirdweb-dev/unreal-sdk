#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IHttpRouter.h"
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

    // PRIVATE KEY WALLET FUNCTIONS

    // Blueprint callable function to create a private key wallet
    UFUNCTION(BlueprintCallable, Category = "Thirdweb|PrivateKeyWallet")
    void CreatePrivateKeyWallet(const FString &PrivateKey, bool &Success, bool &CanRetry, FString &Output);

    // Blueprint callable function to generate a private key wallet
    UFUNCTION(BlueprintCallable, Category = "Thirdweb|PrivateKeyWallet")
    void GeneratePrivateKeyWallet(bool &Success, bool &CanRetry, FString &Output);

    // IN-APP WALLET FUNCTIONS

    // Blueprint callable function to create an InAppWallet
    UFUNCTION(BlueprintCallable, Category = "Thirdweb|InAppWallet")
    void CreateInAppWallet(const FString &Email, const FString &OAuthMethod, bool &Success, bool &CanRetry, FString &Output);

    // Blueprint callable function to send OTP
    UFUNCTION(BlueprintCallable, Category = "Thirdweb|InAppWallet")
    void SendOTP(int64 InAppWalletHandle, bool &Success, bool &CanRetry, FString &Output);

    // Blueprint callable function to verify OTP
    UFUNCTION(BlueprintCallable, Category = "Thirdweb|InAppWallet")
    void VerifyOTP(int64 InAppWalletHandle, const FString &OTP, bool &Success, bool &CanRetry, FString &Output);

    // Blueprint callable function to fetch OAuth login link
    UFUNCTION(BlueprintCallable, Category = "Thirdweb|InAppWallet")
    void FetchOAuthLoginLink(int64 InAppWalletHandle, const FString &RedirectUrl, bool &Success, bool &CanRetry, FString &Output);

    // Blueprint callable function to sign in with OAuth
    UFUNCTION(BlueprintCallable, Category = "Thirdweb|InAppWallet")
    void SignInWithOAuth(int64 InAppWalletHandle, const FString &AuthResult, bool &Success, bool &CanRetry, FString &Output);

    // Blueprint callable function to start OAuth login flow from start to finish
    UFUNCTION(BlueprintCallable, Category = "Thirdweb|InAppWallet")
    void LoginWithOauthDefault(int64 InAppWalletHandle, bool &Success, bool &CanRetry, FString &Output);

    // SMART WALLET FUNCTIONS

    UFUNCTION(BlueprintCallable, Category = "Thirdweb|SmartWallet")
    void CreateSmartWallet(int64 PersonalWalletHandle,
                           const FString &ChainID,
                           bool Gasless,
                           const FString &Factory,
                           const FString &AccountOverride,
                           bool &Success,
                           bool &CanRetry,
                           FString &Output);

    // Blueprint callable function to check if a smart wallet is deployed
    UFUNCTION(BlueprintCallable, Category = "Thirdweb|SmartWallet")
    void IsSmartWalletDeployed(int64 SmartWalletHandle, bool &Success, bool &CanRetry, FString &Output);

    // Blueprint callable function to get all admins of a smart wallet
    UFUNCTION(BlueprintCallable, Category = "Thirdweb|SmartWallet")
    void GetSmartWalletAdmins(int64 SmartWalletHandle, bool &Success, bool &CanRetry, FString &Output);

    // Blueprint callable function to get all active signers of a smart wallet
    UFUNCTION(BlueprintCallable, Category = "Thirdweb|SmartWallet")
    void GetSmartWalletActiveSigners(int64 SmartWalletHandle, bool &Success, bool &CanRetry, FString &Output);

    // Blueprint callable function to create a session key for a smart wallet
    UFUNCTION(BlueprintCallable, Category = "Thirdweb|SmartWallet")
    void CreateSmartWalletSessionKey(int64 SmartWalletHandle,
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
                                     FString &Output);

    // THIRDWEB WALLET FUNCTIONS

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

    // CLIENT CONFIGURATION

    // Properties for configurable parameters
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thirdweb")
    FString ClientID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thirdweb")
    FString BundleID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thirdweb")
    FString SecretKey;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thirdweb")
    FString StorageDirectoryPath;

private:
    void CheckOAuthCompletion();
    FEvent *AuthEvent;
    bool bAuthComplete;
    FString AuthRes;
    int64 OAuthWalletHandle;
    FHttpRouteHandle RouteHandle;
    FString LoginUrl;
    FString OutputString;
    bool bSuccess;
    bool bCanRetry;
};
