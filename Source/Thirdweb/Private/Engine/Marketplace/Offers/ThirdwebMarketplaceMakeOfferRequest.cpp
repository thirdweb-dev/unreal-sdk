// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "Engine/Marketplace/Offers/ThirdwebMarketplaceMakeOfferRequest.h"

#include "Dom/JsonObject.h"

TSharedPtr<FJsonObject> FThirdwebMarketplaceMakeOfferRequest::ToJson() const
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);

	// Required
	JsonObject->SetStringField(TEXT("assetContractAddress"), AssetContractAddress);
	JsonObject->SetStringField(TEXT("tokenId"), TokenId);
	JsonObject->SetStringField(TEXT("totalPrice"), TotalPrice);
	if (!CurrencyContractAddress.IsEmpty())
	{
		JsonObject->SetStringField(TEXT("currencyContractAddress"), CurrencyContractAddress);
	}
	if (!Quantity.IsEmpty())
	{
		JsonObject->SetStringField(TEXT("quantity"), Quantity);
	}
	if (EndTimestamp.ToUnixTimestamp() > 0)
	{
		JsonObject->SetNumberField(TEXT("endTimestamp"), EndTimestamp.ToUnixTimestamp());
	}
	return JsonObject;
}
