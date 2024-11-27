// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "Engine/Marketplace/EnglishAuctions/ThirdwebMarketplaceBid.h"

FThirdwebMarketplaceBid FThirdwebMarketplaceBid::FromJson(const TSharedPtr<FJsonObject>& JsonObject)
{
	FThirdwebMarketplaceBid Bid;
	if (JsonObject.IsValid())
	{
		if (JsonObject->HasTypedField<EJson::String>(TEXT("auctionId")))
		{
			Bid.AuctionId = JsonObject->GetStringField(TEXT("auctionId"));
		}
		if (JsonObject->HasTypedField<EJson::String>(TEXT("bidderAddress")))
		{
			Bid.BidderAddress = JsonObject->GetStringField(TEXT("bidderAddress"));
		}
		if (JsonObject->HasTypedField<EJson::String>(TEXT("currencyContractAddress")))
		{
			Bid.CurrencyContractAddress = JsonObject->GetStringField(TEXT("currencyContractAddress"));
		}
		if (JsonObject->HasTypedField<EJson::String>(TEXT("bidAmount")))
		{
			Bid.BidAmount = JsonObject->GetStringField(TEXT("bidAmount"));
		}
		if (JsonObject->HasTypedField<EJson::Object>(TEXT("bidAmountCurrencyValue")))
		{
			Bid.BidAmountCurrencyValue = FThirdwebAssetCurrencyValue::FromJson(JsonObject->GetObjectField(TEXT("bidAmountCurrencyValue")));
		}
		
	}
	return Bid;
}

TArray<FThirdwebMarketplaceBid> FThirdwebMarketplaceBid::FromJson(const TArray<TSharedPtr<FJsonValue>>& JsonArray)
{
	TArray<FThirdwebMarketplaceBid> Bids;
	for (const TSharedPtr<FJsonValue>& Value : JsonArray)
	{
		if (Value->Type == EJson::Object)
		{
			Bids.Add(FromJson(Value->AsObject()));
		}
	}
	return Bids;
}
