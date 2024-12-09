// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "Engine/ThirdwebAsset.h"
#include "Engine/Marketplace/ThirdwebMarketplaceInternalEntryBase.h"
#include "Misc/DateTime.h"
#include "ThirdwebMarketplaceEnglishAuction.generated.h"

USTRUCT(BlueprintType, DisplayName="Marketplace English Auction")
struct THIRDWEB_API FThirdwebMarketplaceEnglishAuction : public FThirdwebMarketplaceInternalEntryBase
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite, Category="Auction")
	FString MinimumBidAmount;

	UPROPERTY(BlueprintReadWrite, Category="Auction")
	FString BuyoutBidAmount;
	
	UPROPERTY(BlueprintReadWrite, Category="Auction")
	FThirdwebAssetCurrencyValue BuyoutCurrencyValue;

	UPROPERTY(BlueprintReadWrite, Category="Auction")
	FDateTime TimeBufferInSeconds;

	UPROPERTY(BlueprintReadWrite, Category="Auction")
	int32 BidBufferBps = 0;
	
	static FThirdwebMarketplaceEnglishAuction FromJson(const TSharedPtr<FJsonObject>& JsonObject);
	static TArray<FThirdwebMarketplaceEnglishAuction> FromJson(const TArray<TSharedPtr<FJsonValue>>& JsonArray);

	friend uint32 GetTypeHash(const FThirdwebMarketplaceEnglishAuction& Other)
	{
		return FCrc::MemCrc32(&Other, sizeof(FThirdwebMarketplaceEnglishAuction));
	}
};

