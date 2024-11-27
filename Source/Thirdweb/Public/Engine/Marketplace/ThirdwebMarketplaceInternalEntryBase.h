// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "Engine/ThirdwebAsset.h"
#include "ThirdwebMarketplaceInternalEntryBase.generated.h"

class FJsonObject;
class FJsonValue;
enum class EThirdwebMarketplaceListingStatus : uint8;

USTRUCT(BlueprintType)
struct FThirdwebMarketplaceInternalEntryBase
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category="Core")
	FString AssetContractAddress;

	UPROPERTY(BlueprintReadWrite, Category="Core")
	FString TokenId;

	UPROPERTY(BlueprintReadWrite, Category="Core")
	FString CurrencyContractAddress;

	UPROPERTY(BlueprintReadWrite, Category="Core")
	FString Quantity;

	UPROPERTY(BlueprintReadWrite, Category="Core")
	FString Id;

	UPROPERTY(BlueprintReadWrite, Category="Core")
	FThirdwebAsset Asset;

	UPROPERTY(BlueprintReadWrite, Category="Core")
	EThirdwebMarketplaceListingStatus Status;

	UPROPERTY(BlueprintReadWrite, Category="Core")
	FDateTime StartTimeInSeconds;

	UPROPERTY(BlueprintReadWrite, Category="Core")
	FDateTime EndTimeInSeconds;
	
	static FThirdwebMarketplaceInternalEntryBase FromJson(const TSharedPtr<FJsonObject>& JsonObject);
	static TArray<FThirdwebMarketplaceInternalEntryBase> FromJson(const TArray<TSharedPtr<FJsonValue>>& JsonArray);

protected:
	void Load(const FThirdwebMarketplaceInternalEntryBase& Base);
};