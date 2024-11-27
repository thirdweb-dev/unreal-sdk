// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "Engine/Marketplace/ThirdwebMarketplaceInternalEntryBase.h"
#include "ThirdwebMarketplaceOffer.generated.h"

class FJsonObject;
class FJsonValue;

USTRUCT(BlueprintType, DisplayName="Marketplace Offer")
struct THIRDWEB_API FThirdwebMarketplaceOffer : public FThirdwebMarketplaceInternalEntryBase
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite, Category="Offer")
	FString OfferorAddress;
	
	UPROPERTY(BlueprintReadWrite, Category="Offer")
	FThirdwebAssetCurrencyValue CurrencyValue;

	UPROPERTY(BlueprintReadWrite, Category="Offer")
	FString TotalPrice;
	
	static FThirdwebMarketplaceOffer FromJson(const TSharedPtr<FJsonObject>& JsonObject);
	static TArray<FThirdwebMarketplaceOffer> FromJson(const TArray<TSharedPtr<FJsonValue>>& JsonArray);
};

