// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "Engine/Marketplace/DirectListings/ThirdwebMarketplaceListing.h"

#include "Engine/Marketplace/ThirdwebMarketplaceCommon.h"

FThirdwebAssetCurrencyValue FThirdwebAssetCurrencyValue::FromJson(const TSharedPtr<FJsonObject>& JsonObject)
{
	FThirdwebAssetCurrencyValue CurrencyValue;
	if (JsonObject.IsValid())
	{
		if (JsonObject->HasTypedField<EJson::String>(TEXT("name")))
		{
			CurrencyValue.Name = JsonObject->GetStringField(TEXT("name"));
		}
		if (JsonObject->HasTypedField<EJson::String>(TEXT("symbol")))
		{
			CurrencyValue.Symbol = JsonObject->GetStringField(TEXT("symbol"));
		}
		if (JsonObject->HasTypedField<EJson::Number>(TEXT("decimals")))
		{
			CurrencyValue.Decimals = JsonObject->GetIntegerField(TEXT("decimals"));
		}
		if (JsonObject->HasTypedField<EJson::String>(TEXT("value")))
		{
			CurrencyValue.Value = JsonObject->GetStringField(TEXT("value"));
		}
		if (JsonObject->HasTypedField<EJson::String>(TEXT("displayValue")))
		{
			CurrencyValue.DisplayValue = JsonObject->GetStringField(TEXT("displayValue"));
		}
	}
	return CurrencyValue;
}

FThirdwebAssetAttribute FThirdwebAssetAttribute::FromJson(const TSharedPtr<FJsonObject>& JsonObject)
{
	FThirdwebAssetAttribute Attribute;
	if (JsonObject.IsValid())
	{
		if (JsonObject->HasTypedField<EJson::String>(TEXT("trait_type")))
		{
			Attribute.TraitType = JsonObject->GetStringField(TEXT("trait_type"));
		}
		if (JsonObject->HasTypedField<EJson::String>(TEXT("value")))
		{
			Attribute.Value = JsonObject->GetStringField(TEXT("value"));
		}
	}
	return Attribute;
}

TArray<FThirdwebAssetAttribute> FThirdwebAssetAttribute::FromJson(const TArray<TSharedPtr<FJsonValue>>& JsonArray)
{
	TArray<FThirdwebAssetAttribute> Attributes;
	for (const TSharedPtr<FJsonValue>& Value : JsonArray)
	{
		if (Value->Type == EJson::Object)
		{
			Attributes.Add(FromJson(Value->AsObject()));
		}
	}
	return Attributes;
}

FThirdwebAsset FThirdwebAsset::FromJson(const TSharedPtr<FJsonObject>& JsonObject)
{
	FThirdwebAsset Asset;
	if (JsonObject.IsValid())
	{
		if (JsonObject->HasTypedField<EJson::String>(TEXT("id")))
		{
			Asset.Id = JsonObject->GetStringField(TEXT("id"));
		}
		if (JsonObject->HasTypedField<EJson::String>(TEXT("uri")))
		{
			Asset.Uri = JsonObject->GetStringField(TEXT("uri"));
		}
		if (JsonObject->HasTypedField<EJson::String>(TEXT("name")))
		{
			Asset.Name = JsonObject->GetStringField(TEXT("name"));
		}
		if (JsonObject->HasTypedField<EJson::String>(TEXT("description")))
		{
			Asset.Description = JsonObject->GetStringField(TEXT("description"));
		}
		if (JsonObject->HasTypedField<EJson::String>(TEXT("external_url")))
		{
			Asset.ExternalUrl = JsonObject->GetStringField(TEXT("external_url"));
		}
		if (JsonObject->HasTypedField<EJson::Array>(TEXT("attributes")))
		{
			Asset.Attributes = FThirdwebAssetAttribute::FromJson(JsonObject->GetArrayField(TEXT("attributes")));
		}
	}
	return Asset;
}

FThirdwebMarketplaceInternalBaseAuction FThirdwebMarketplaceInternalBaseAuction::FromJson(const TSharedPtr<FJsonObject>& JsonObject)
{
	FThirdwebMarketplaceInternalBaseAuction Base = {};
	if (JsonObject.IsValid())
	{
		if (JsonObject->HasTypedField<EJson::String>(TEXT("assetContractAddress")))
		{
			Base.AssetContractAddress = JsonObject->GetStringField(TEXT("assetContractAddress"));
		}
		if (JsonObject->HasTypedField<EJson::String>(TEXT("tokenId")))
		{
			Base.TokenId = JsonObject->GetStringField(TEXT("tokenId"));
		}
		if (JsonObject->HasTypedField<EJson::String>(TEXT("currencyContractAddress")))
		{
			Base.CurrencyContractAddress = JsonObject->GetStringField(TEXT("currencyContractAddress"));
		}
		if (JsonObject->HasTypedField<EJson::String>(TEXT("quantity")))
		{
			Base.Quantity = JsonObject->GetStringField(TEXT("quantity"));
		}
		if (JsonObject->HasTypedField<EJson::String>(TEXT("id")))
		{
			Base.Id = JsonObject->GetStringField(TEXT("id"));
		}
		if (JsonObject->HasTypedField<EJson::Object>(TEXT("asset")))
		{
			Base.Asset = FThirdwebAsset::FromJson(JsonObject->GetObjectField(TEXT("asset")));
		}
		if (JsonObject->HasTypedField<EJson::Number>(TEXT("status")))
		{
			int32 Status = JsonObject->GetIntegerField(TEXT("status"));
			Base.Status = Status >= 0 && Status <= static_cast<int32>(EThirdwebMarketplaceListingStatus::Max)
				                 ? static_cast<EThirdwebMarketplaceListingStatus>(Status)
				                 : EThirdwebMarketplaceListingStatus::Invalid;
		}
		if (JsonObject->HasTypedField<EJson::Number>(TEXT("startTimeInSeconds")))
		{
			Base.StartTimeInSeconds = FDateTime::FromUnixTimestamp(JsonObject->GetNumberField(TEXT("startTimeInSeconds")));
		}
		if (JsonObject->HasTypedField<EJson::Number>(TEXT("endTimeInSeconds")))
		{
			Base.EndTimeInSeconds = FDateTime::FromUnixTimestamp(JsonObject->GetNumberField(TEXT("endTimeInSeconds")));
		}
	}
	return Base;
}

TArray<FThirdwebMarketplaceInternalBaseAuction> FThirdwebMarketplaceInternalBaseAuction::FromJson(const TArray<TSharedPtr<FJsonValue>>& JsonArray)
{
	TArray<FThirdwebMarketplaceInternalBaseAuction> Bases;
	for (const TSharedPtr<FJsonValue>& Value : JsonArray)
	{
		
		if (Value->Type == EJson::Object)
		{
			Bases.Add(FromJson(Value->AsObject()));
		}
	}
	return Bases;
}

FThirdwebMarketplaceListing FThirdwebMarketplaceListing::FromJson(const TSharedPtr<FJsonObject>& JsonObject)
{
	FThirdwebMarketplaceListing Listing = FThirdwebMarketplaceListing(Super::FromJson(JsonObject));
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

TArray<FThirdwebMarketplaceListing> FThirdwebMarketplaceListing::FromJson(const TArray<TSharedPtr<FJsonValue>>& JsonArray)
{
	TArray<FThirdwebMarketplaceListing> Listings;
	for (const TSharedPtr<FJsonValue>& Value : JsonArray)
	{
		if (Value->Type == EJson::Object)
		{
			Listings.Add(FromJson(Value->AsObject()));
		}
	}
	return Listings;
}

FThirdwebMarketplaceEnglishAuction FThirdwebMarketplaceEnglishAuction::FromJson(const TSharedPtr<FJsonObject>& JsonObject)
{
	FThirdwebMarketplaceEnglishAuction Auction = FThirdwebMarketplaceEnglishAuction(Super::FromJson(JsonObject));
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

FThirdwebMarketplaceOffer FThirdwebMarketplaceOffer::FromJson(const TSharedPtr<FJsonObject>& JsonObject)
{
	FThirdwebMarketplaceOffer Offer = FThirdwebMarketplaceOffer(Super::FromJson(JsonObject));
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
