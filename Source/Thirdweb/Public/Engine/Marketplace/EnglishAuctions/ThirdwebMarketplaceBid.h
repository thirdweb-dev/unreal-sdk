// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "Engine/ThirdwebAsset.h"
#include "ThirdwebMarketplaceBid.generated.h"

class FJsonObject;
class FJsonValue;

USTRUCT(BlueprintType, DisplayName="Marketplace Bid")
struct THIRDWEB_API FThirdwebMarketplaceBid
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite, Category="Bid")
	FString AuctionId;
	
	UPROPERTY(BlueprintReadWrite, Category="Bid")
	FString BidderAddress;

	UPROPERTY(BlueprintReadWrite, Category="Bid")
	FString CurrencyContractAddress;

	UPROPERTY(BlueprintReadWrite, Category="Bid")
	FString BidAmount;

	UPROPERTY(BlueprintReadWrite, Category="Bid")
	FThirdwebAssetCurrencyValue BidAmountCurrencyValue;
	
	static FThirdwebMarketplaceBid FromJson(const TSharedPtr<FJsonObject>& JsonObject);
	static TArray<FThirdwebMarketplaceBid> FromJson(const TArray<TSharedPtr<FJsonValue>>& JsonArray);
};