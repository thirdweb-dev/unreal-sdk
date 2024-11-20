// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "Misc/DateTime.h"
#include "ThirdwebAccountIdentifierParams.generated.h"

USTRUCT(BlueprintType, DisplayName="Account Identifier Params")
struct THIRDWEB_API FThirdwebAccountIdentifierParams
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Thirdweb|Account")
	FString Address;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Thirdweb|Account")
	FString FactoryAddress;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Thirdweb|Account")
	FString Salt;
};


USTRUCT(BlueprintType, DisplayName="Create Listing Request")
struct THIRDWEB_API FThirdwebMarketplaceCreateListingRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Thirdweb|Marketplace|Direct Listings")
	FString AssetContractAddress;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Thirdweb|Marketplace|Direct Listings")
	FString TokenId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Thirdweb|Marketplace|Direct Listings")
	FString PricePerToken;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Thirdweb|Marketplace|Direct Listings")
	bool bIsReservedListing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Thirdweb|Marketplace|Direct Listings")
	FString Quantity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Thirdweb|Marketplace|Direct Listings")
    FDateTime StartTimestamp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Thirdweb|Marketplace|Direct Listings")
	FDateTime EndTimestamp;
};

USTRUCT(BlueprintType, DisplayName="Create Auction Request")
struct THIRDWEB_API FThirdwebMarketplaceCreateAuctionRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Thirdweb|Marketplace|English Auctions")
	FString AssetContractAddress;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Thirdweb|Marketplace|English Auctions")
	FString TokenId;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Thirdweb|Marketplace|English Auctions")
	FString Quantity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Thirdweb|Marketplace|English Auctions")
	FDateTime StartTimestamp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Thirdweb|Marketplace|English Auctions")
	FDateTime EndTimestamp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Thirdweb|Marketplace|English Auctions")
	FString BuyoutBidAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Thirdweb|Marketplace|English Auctions")
	FString MinimumBidAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Thirdweb|Marketplace|English Auctions")
	FString CurrencyContractAddress;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Thirdweb|Marketplace|English Auctions")
	int32 BidBufferBps;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Thirdweb|Marketplace|English Auctions")
	int32 TimeBufferInSeconds;
};

USTRUCT(BlueprintType, DisplayName="Create Auction Request")
struct THIRDWEB_API FThirdwebMarketplaceMakeOfferRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Thirdweb|Marketplace|Offers")
	FString AssetContractAddress;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Thirdweb|Marketplace|Offers")
	FString TokenId;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Thirdweb|Marketplace|Offers")
	FString Quantity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Thirdweb|Marketplace|Offers")
	FDateTime EndTimestamp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Thirdweb|Marketplace|Offers")
	FString CurrencyContractAddress;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Thirdweb|Marketplace|Offers")
	FString TotalPrice;

};