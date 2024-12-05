// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "Engine/Marketplace/EnglishAuctions/ThirdwebMarketplaceCreateEnglishAuctionRequest.h"

#include "Dom/JsonObject.h"

TSharedPtr<FJsonObject> FThirdwebMarketplaceCreateEnglishAuctionRequest::ToJson() const
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);

	JsonObject->SetStringField(TEXT("assetContractAddress"), AssetContractAddress);
	JsonObject->SetStringField(TEXT("tokenId"), TokenId);
	JsonObject->SetStringField(TEXT("buyoutBidAmount"), BuyoutBidAmount);
	JsonObject->SetStringField(TEXT("minimumBidAmount"), MinimumBidAmount);

	if (!CurrencyContractAddress.IsEmpty())
	{
		JsonObject->SetStringField(TEXT("currencyContractAddress"), CurrencyContractAddress);
	}
	if (!Quantity.IsEmpty())
	{
		JsonObject->SetStringField(TEXT("quantity"), Quantity);
	}
	if (static_cast<int32>(StartTimestamp.ToUnixTimestamp()) > 0)
	{
		JsonObject->SetStringField(TEXT("startTimestamp"), FString::FromInt(StartTimestamp.ToUnixTimestamp()));
	}
	if (StartTimestamp.ToUnixTimestamp() > 0)
	{
		JsonObject->SetStringField(TEXT("endTimestamp"), FString::FromInt(EndTimestamp.ToUnixTimestamp()));
	}
	if (BidBufferBps > 0)
	{
		JsonObject->SetNumberField(TEXT("bidBufferBps"), BidBufferBps);
	}
	if (TimeBufferInSeconds.GetTotalSeconds() > 0)
	{
		JsonObject->SetNumberField(TEXT("timeBufferInSeconds"), TimeBufferInSeconds.GetTotalSeconds());
	}

	return JsonObject;
}
