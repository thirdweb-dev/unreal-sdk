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

FThirdwebMarketplaceListing FThirdwebMarketplaceListing::FromJson(const TSharedPtr<FJsonObject>& JsonObject)
{
	FThirdwebMarketplaceListing Listing = {};
	if (JsonObject.IsValid())
	{
		if (JsonObject->HasTypedField<EJson::String>(TEXT("assetContractAddress")))
		{
			Listing.AssetContractAddress = JsonObject->GetStringField(TEXT("assetContractAddress"));
		}
		if (JsonObject->HasTypedField<EJson::String>(TEXT("tokenId")))
		{
			Listing.TokenId = JsonObject->GetStringField(TEXT("tokenId"));
		}
		if (JsonObject->HasTypedField<EJson::String>(TEXT("currencyContractAddress")))
		{
			Listing.CurrencyContractAddress = JsonObject->GetStringField(TEXT("currencyContractAddress"));
		}
		if (JsonObject->HasTypedField<EJson::String>(TEXT("quantity")))
		{
			Listing.Quantity = JsonObject->GetStringField(TEXT("quantity"));
		}
		if (JsonObject->HasTypedField<EJson::String>(TEXT("pricePerToken")))
		{
			Listing.PricePerToken = JsonObject->GetStringField(TEXT("pricePerToken"));
		}
		if (JsonObject->HasTypedField<EJson::Boolean>(TEXT("isReservedListing")))
		{
			Listing.bIsReservedListing = JsonObject->GetBoolField(TEXT("isReservedListing"));
		}
		if (JsonObject->HasTypedField<EJson::String>(TEXT("id")))
		{
			Listing.Id = JsonObject->GetStringField(TEXT("id"));
		}
		if (JsonObject->HasTypedField<EJson::Object>(TEXT("currencyValuePerToken")))
		{
			Listing.CurrencyValuePerToken = FThirdwebAssetCurrencyValue::FromJson(JsonObject->GetObjectField(TEXT("currencyValuePerToken")));
		}
		if (JsonObject->HasTypedField<EJson::Object>(TEXT("asset")))
		{
			Listing.Asset = FThirdwebAsset::FromJson(JsonObject->GetObjectField(TEXT("asset")));
		}
		if (JsonObject->HasTypedField<EJson::Number>(TEXT("status")))
		{
			int32 Status = JsonObject->GetIntegerField(TEXT("status"));
			Listing.Status = Status >= 0 && Status <= static_cast<int32>(EThirdwebMarketplaceListingStatus::Max)
				                 ? static_cast<EThirdwebMarketplaceListingStatus>(Status)
				                 : EThirdwebMarketplaceListingStatus::Invalid;
		}
		if (JsonObject->HasTypedField<EJson::Number>(TEXT("startTimeInSeconds")))
		{
			Listing.StartTimeInSeconds = FDateTime::FromUnixTimestamp(JsonObject->GetNumberField(TEXT("startTimeInSeconds")));
		}
		if (JsonObject->HasTypedField<EJson::Number>(TEXT("endTimeInSeconds")))
		{
			Listing.EndTimeInSeconds = FDateTime::FromUnixTimestamp(JsonObject->GetNumberField(TEXT("endTimeInSeconds")));
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
			Attributes.Add(FromJson(Value->AsObject()));
		}
	}
	return Attributes;
}
