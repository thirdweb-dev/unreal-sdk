// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "Engine/Marketplace/Offers/ThirdwebMarketplaceOffer.h"

#include "Dom/JsonObject.h"
#include "Engine/Marketplace/ThirdwebMarketplaceCommon.h"
#include "Misc/DateTime.h"

FThirdwebMarketplaceOffer FThirdwebMarketplaceOffer::FromJson(const TSharedPtr<FJsonObject>& JsonObject)
{
	FThirdwebMarketplaceOffer Offer;
	Offer.Load(Super::FromJson(JsonObject));
	if (JsonObject.IsValid())
	{
		if (JsonObject->HasTypedField<EJson::String>(TEXT("offerorAddress")))
		{
			Offer.OfferorAddress = JsonObject->GetStringField(TEXT("offerorAddress"));
		}
		if (JsonObject->HasTypedField<EJson::Object>(TEXT("currencyValue")))
		{
			Offer.CurrencyValue = FThirdwebAssetCurrencyValue::FromJson(JsonObject->GetObjectField(TEXT("currencyValue")));
		}
		if (JsonObject->HasTypedField<EJson::String>(TEXT("totalPrice")))
		{
			Offer.TotalPrice = JsonObject->GetStringField(TEXT("totalPrice"));
		}
	}
	return Offer;
}

TArray<FThirdwebMarketplaceOffer> FThirdwebMarketplaceOffer::FromJson(const TArray<TSharedPtr<FJsonValue>>& JsonArray)
{
	TArray<FThirdwebMarketplaceOffer> Offers;
	for (const TSharedPtr<FJsonValue>& Value : JsonArray)
	{
		if (Value->Type == EJson::Object)
		{
			Offers.Add(FromJson(Value->AsObject()));
		}
	}
	return Offers;
}
