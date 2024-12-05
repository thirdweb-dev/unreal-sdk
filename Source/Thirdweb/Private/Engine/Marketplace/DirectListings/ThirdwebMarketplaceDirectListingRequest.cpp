// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "Engine/Marketplace/DirectListings/ThirdwebMarketplaceDirectListingRequest.h"

#include "Dom/JsonObject.h"
#include "Engine/Marketplace/ThirdwebMarketplaceCommon.h"

TSharedPtr<FJsonObject> FThirdwebMarketplaceCreateDirectListingRequest::ToJson() const
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);

	// Required
	JsonObject->SetStringField(TEXT("assetContractAddress"), AssetContractAddress);
	JsonObject->SetStringField(TEXT("tokenId"), TokenId);
	JsonObject->SetStringField(TEXT("pricePerToken"), PricePerToken);
	JsonObject->SetBoolField(TEXT("isReservedListing"), bIsReservedListing);
	// Optional
	if (!CurrencyContractAddress.IsEmpty())
	{
		JsonObject->SetStringField(TEXT("currencyContractAddress"), CurrencyContractAddress);
	}
	JsonObject->SetBoolField(TEXT("isReservedListing"), bIsReservedListing);
	if (!Quantity.IsEmpty())
	{
		JsonObject->SetStringField(TEXT("quantity"), Quantity);
	}
	if (StartTimestamp.ToUnixTimestamp() > 0)
	{
		JsonObject->SetNumberField(TEXT("startTimestamp"), StartTimestamp.ToUnixTimestamp());
	}
	if (EndTimestamp.ToUnixTimestamp() > 0)
	{
		JsonObject->SetNumberField(TEXT("endTimestamp"), EndTimestamp.ToUnixTimestamp());
	}
	
	return JsonObject;
}

TSharedPtr<FJsonObject> FThirdwebMarketplaceUpdateDirectListingRequest::ToJson() const
{
	TSharedPtr<FJsonObject> JsonObject = Super::ToJson();
	JsonObject->SetStringField(TEXT("listingId"), ListingId);
	return JsonObject;
}
