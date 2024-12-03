// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "Engine/ThirdwebAsset.h"

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
