// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "Engine/Marketplace/DirectListings/ThirdwebMarketplaceDirectListing.h"

#include "Dom/JsonObject.h"
#include "Engine/Marketplace/ThirdwebMarketplaceCommon.h"

FThirdwebMarketplaceDirectListing FThirdwebMarketplaceDirectListing::FromJson(const TSharedPtr<FJsonObject>& JsonObject)
{
	FThirdwebMarketplaceDirectListing Listing;
	Listing.Load(Super::FromJson(JsonObject));

	if (JsonObject.IsValid())
	{
		if (JsonObject->HasTypedField<EJson::String>(TEXT("pricePerToken")))
		{
			Listing.PricePerToken = JsonObject->GetStringField(TEXT("pricePerToken"));
		}
		if (JsonObject->HasTypedField<EJson::Boolean>(TEXT("isReservedListing")))
		{
			Listing.bIsReservedListing = JsonObject->GetBoolField(TEXT("isReservedListing"));
		}
		if (JsonObject->HasTypedField<EJson::Object>(TEXT("currencyValuePerToken")))
		{
			Listing.CurrencyValuePerToken = FThirdwebAssetCurrencyValue::FromJson(JsonObject->GetObjectField(TEXT("currencyValuePerToken")));
		}
	}
	return Listing;
}

TArray<FThirdwebMarketplaceDirectListing> FThirdwebMarketplaceDirectListing::FromJson(const TArray<TSharedPtr<FJsonValue>>& JsonArray)
{
	TArray<FThirdwebMarketplaceDirectListing> Listings;
	for (const TSharedPtr<FJsonValue>& Value : JsonArray)
	{
		if (Value->Type == EJson::Object)
		{
			Listings.Add(FromJson(Value->AsObject()));
		}
	}
	return Listings;
}
