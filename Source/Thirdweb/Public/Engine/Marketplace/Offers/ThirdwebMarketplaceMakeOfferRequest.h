// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "ThirdwebMarketplaceMakeOfferRequest.generated.h"

class FJsonObject;

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

	TSharedPtr<FJsonObject> ToJson() const;
};