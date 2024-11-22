// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "ThirdwebMarketplaceCreateEnglishAuctionRequest.generated.h"

USTRUCT(BlueprintType, DisplayName="Marketplace Create English Auction Request")
struct THIRDWEB_API FThirdwebMarketplaceCreateEnglishAuctionRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Auction")
	FString AssetContractAddress;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Auction")
	FString TokenId;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Auction")
	FString CurrencyContractAddress;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Auction")
	FString Quantity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Auction")
	FDateTime StartTimestamp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Auction")
	FDateTime EndTimestamp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Auction")
	FString BuyoutBidAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Auction")
	FString MinimumBidAmount;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Auction")
	int32 BidBufferBps = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Auction")
	FTimespan TimeBufferInSeconds;

	TSharedPtr<FJsonObject> ToJson() const;
};
