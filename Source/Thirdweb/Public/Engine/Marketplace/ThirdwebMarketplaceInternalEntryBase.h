// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "ThirdwebMarketplaceCommon.h"
#include "Engine/ThirdwebAsset.h"
#include "ThirdwebMarketplaceInternalEntryBase.generated.h"

class FJsonObject;
class FJsonValue;

USTRUCT(BlueprintType)
struct THIRDWEB_API FThirdwebMarketplaceInternalEntryBase
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

	friend uint32 GetTypeHash(const FThirdwebMarketplaceInternalEntryBase& Other)
   	{
  		return FCrc::MemCrc32(&Other, sizeof(FThirdwebMarketplaceInternalEntryBase));
  	}
protected:
	void Load(const FThirdwebMarketplaceInternalEntryBase& Base);
};