// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "Engine/Marketplace/ThirdwebMarketplaceInternalEntryBase.h"

#include "Engine/Marketplace/ThirdwebMarketplaceCommon.h"


void FThirdwebMarketplaceInternalEntryBase::Load(const FThirdwebMarketplaceInternalEntryBase& Base)
{
	AssetContractAddress = Base.AssetContractAddress;
	TokenId = Base.TokenId;
	CurrencyContractAddress = Base.CurrencyContractAddress;
	Quantity = Base.Quantity;
	Id = Base.Id;
	Asset = Base.Asset;
	Status = Base.Status;
	StartTimeInSeconds = Base.StartTimeInSeconds;
	EndTimeInSeconds = Base.EndTimeInSeconds;
}

FThirdwebMarketplaceInternalEntryBase FThirdwebMarketplaceInternalEntryBase::FromJson(const TSharedPtr<FJsonObject>& JsonObject)
{
	FThirdwebMarketplaceInternalEntryBase Base = {};
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

TArray<FThirdwebMarketplaceInternalEntryBase> FThirdwebMarketplaceInternalEntryBase::FromJson(const TArray<TSharedPtr<FJsonValue>>& JsonArray)
{
	TArray<FThirdwebMarketplaceInternalEntryBase> Bases;
	for (const TSharedPtr<FJsonValue>& Value : JsonArray)
	{
		
		if (Value->Type == EJson::Object)
		{
			Bases.Add(FromJson(Value->AsObject()));
		}
	}
	return Bases;
}