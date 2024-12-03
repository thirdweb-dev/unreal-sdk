#pragma once

#include "ThirdwebBackendWallet.generated.h"

class FJsonObject;
class FJsonValue;

USTRUCT(BlueprintType)
struct THIRDWEB_API FThirdwebBackendWallet
{
	GENERATED_BODY()
    
	UPROPERTY(BlueprintReadWrite, Category="Backend Wallet")
	FString Address;

	UPROPERTY(BlueprintReadWrite, Category="Backend Wallet")
	FString Type;

	UPROPERTY(BlueprintReadWrite, Category="Backend Wallet")
	FString Label;

	UPROPERTY(BlueprintReadWrite, Category="Backend Wallet")
	FString AwsKmsKeyId;

	UPROPERTY(BlueprintReadWrite, Category="Backend Wallet")
	FString AwsKmsArn;

	UPROPERTY(BlueprintReadWrite, Category="Backend Wallet")
	FString GcpKmsKeyId;

	UPROPERTY(BlueprintReadWrite, Category="Backend Wallet")
	FString GcpKmsKeyRingId;

	UPROPERTY(BlueprintReadWrite, Category="Backend Wallet")
	FString GcpKmsLocationId;

	UPROPERTY(BlueprintReadWrite, Category="Backend Wallet")
	FString GcpKmsKeyVersionId;

	UPROPERTY(BlueprintReadWrite, Category="Backend Wallet")
	FString GcpKmsResourcePath;

	static FThirdwebBackendWallet FromJson(const TSharedPtr<FJsonObject>& JsonObject);
	static TArray<FThirdwebBackendWallet> FromJson(const TArray<TSharedPtr<FJsonValue>>& JsonArray);
};

