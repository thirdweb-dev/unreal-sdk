// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "Engine/Marketplace/EnglishAuctions/ThirdwebMarketplaceEnglishAuction.h"

#include "Engine/Marketplace/ThirdwebMarketplaceCommon.h"


FThirdwebMarketplaceEnglishAuction FThirdwebMarketplaceEnglishAuction::FromJson(const TSharedPtr<FJsonObject>& JsonObject)
{
	FThirdwebMarketplaceEnglishAuction Auction;
	Auction.Load(Super::FromJson(JsonObject));
	
	if (JsonObject.IsValid())
	{
		if (JsonObject->HasTypedField<EJson::String>(TEXT("minimumBidAmount")))
		{
			Auction.MinimumBidAmount = JsonObject->GetStringField(TEXT("minimumBidAmount"));
		}
		if (JsonObject->HasTypedField<EJson::String>(TEXT("buyoutBidAmount")))
		{
			Auction.BuyoutBidAmount = JsonObject->GetStringField(TEXT("buyoutBidAmount"));
		}
		if (JsonObject->HasTypedField<EJson::Object>(TEXT("buyoutCurrencyValue")))
		{
			Auction.BuyoutCurrencyValue = FThirdwebAssetCurrencyValue::FromJson(JsonObject->GetObjectField(TEXT("buyoutCurrencyValue")));
		}
		if (JsonObject->HasTypedField<EJson::Number>(TEXT("timeBufferInSeconds")))
		{
			Auction.TimeBufferInSeconds = FDateTime::FromUnixTimestamp(JsonObject->GetNumberField(TEXT("timeBufferInSeconds")));
		}
		if (JsonObject->HasTypedField<EJson::Number>(TEXT("bidBufferBps")))
		{
			Auction.BidBufferBps = JsonObject->GetIntegerField(TEXT("bidBufferBps"));
		}
	}
	return Auction;
}

TArray<FThirdwebMarketplaceEnglishAuction> FThirdwebMarketplaceEnglishAuction::FromJson(const TArray<TSharedPtr<FJsonValue>>& JsonArray)
{
	TArray<FThirdwebMarketplaceEnglishAuction> Auctions;
	for (const TSharedPtr<FJsonValue>& Value : JsonArray)
	{
		if (Value->Type == EJson::Object)
		{
			Auctions.Add(FromJson(Value->AsObject()));
		}
	}
	return Auctions;
}
