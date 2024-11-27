// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "Engine/ThirdwebAsset.h"
#include "Engine/Marketplace/ThirdwebMarketplaceInternalEntryBase.h"
#include "ThirdwebMarketplaceDirectListingRequest.generated.h"

class FJsonObject;
class FJsonValue;

USTRUCT(BlueprintType, DisplayName="Marketplace Create Listing Request")
struct THIRDWEB_API FThirdwebMarketplaceCreateDirectListingRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Listing")
	FString AssetContractAddress;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Listing")
	FString TokenId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Listing")
	FString CurrencyContractAddress;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Listing")
	FString PricePerToken;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Listing")
	bool bIsReservedListing = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Listing")
	FString Quantity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Listing")
	FDateTime StartTimestamp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Listing")
	FDateTime EndTimestamp;

	TSharedPtr<FJsonObject> ToJson() const;
};

USTRUCT(BlueprintType, DisplayName="Marketplace Update Listing Request")
struct THIRDWEB_API FThirdwebMarketplaceUpdateDirectListingRequest : public FThirdwebMarketplaceCreateDirectListingRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Listing")
	FString ListingId;

	TSharedPtr<FJsonObject> ToJson() const;
};
