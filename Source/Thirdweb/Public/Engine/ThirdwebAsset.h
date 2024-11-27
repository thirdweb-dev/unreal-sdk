// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "ThirdwebAsset.generated.h"

class FJsonObject;
class FJsonValue;

USTRUCT(BlueprintType)
struct THIRDWEB_API FThirdwebAssetCurrencyValue
{
	GENERATED_BODY()
    
	UPROPERTY(BlueprintReadWrite, Category="Currency Value")
	FString Name;

	UPROPERTY(BlueprintReadWrite, Category="Currency Value")
	FString Symbol;

	UPROPERTY(BlueprintReadWrite, Category="Currency Value")
	int32 Decimals = 0;

	UPROPERTY(BlueprintReadWrite, Category="Currency Value")
	FString Value;

	UPROPERTY(BlueprintReadWrite, Category="Currency Value")
	FString DisplayValue;

	static FThirdwebAssetCurrencyValue FromJson(const TSharedPtr<FJsonObject>& JsonObject);
};


USTRUCT(BlueprintType)
struct THIRDWEB_API FThirdwebAssetAttribute
{
	GENERATED_BODY()
    
	UPROPERTY(BlueprintReadWrite, Category="Asset Attribute")
	FString TraitType;

	UPROPERTY(BlueprintReadWrite, Category="Asset Attribute")
	FString Value;

	static FThirdwebAssetAttribute FromJson(const TSharedPtr<FJsonObject>& JsonObject);
	static TArray<FThirdwebAssetAttribute> FromJson(const TArray<TSharedPtr<FJsonValue>>& JsonArray);
};


USTRUCT(BlueprintType)
struct THIRDWEB_API FThirdwebAsset
{
	GENERATED_BODY()
    
	UPROPERTY(BlueprintReadWrite, Category="Asset")
	FString Id;

	UPROPERTY(BlueprintReadWrite, Category="Asset")
	FString Uri;

	UPROPERTY(BlueprintReadWrite, Category="Asset")
	FString Name;

	UPROPERTY(BlueprintReadWrite, Category="Asset")
	FString Description;

	UPROPERTY(BlueprintReadWrite, Category="Asset")
	FString ExternalUrl;

	UPROPERTY(BlueprintReadWrite, Category="Asset")
	TArray<FThirdwebAssetAttribute> Attributes;

	static FThirdwebAsset FromJson(const TSharedPtr<FJsonObject>& JsonObject);
};
