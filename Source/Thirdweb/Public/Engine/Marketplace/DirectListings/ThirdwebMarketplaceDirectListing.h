// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "Engine/ThirdwebAsset.h"
#include "Engine/Marketplace/ThirdwebMarketplaceInternalEntryBase.h"
#include "ThirdwebMarketplaceDirectListing.generated.h"

class FJsonObject;
class FJsonValue;

USTRUCT(BlueprintType, DisplayName="Marketplace Listing")
struct THIRDWEB_API FThirdwebMarketplaceDirectListing : public FThirdwebMarketplaceInternalEntryBase
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite, Category="Listing")
	FString PricePerToken;

	UPROPERTY(BlueprintReadWrite, Category="Listing")
	bool bIsReservedListing = false;
	
	UPROPERTY(BlueprintReadWrite, Category="Listing")
	FThirdwebAssetCurrencyValue CurrencyValuePerToken;

	static FThirdwebMarketplaceDirectListing FromJson(const TSharedPtr<FJsonObject>& JsonObject);
	static TArray<FThirdwebMarketplaceDirectListing> FromJson(const TArray<TSharedPtr<FJsonValue>>& JsonArray);

	friend uint32 GetTypeHash(const FThirdwebMarketplaceDirectListing& Other)
	{
		return FCrc::MemCrc32(&Other, sizeof(FThirdwebMarketplaceDirectListing));
	}
};

