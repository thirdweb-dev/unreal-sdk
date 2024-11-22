// AssetCurrencyValue.h
#pragma once

#include "ThirdwebMarketplaceListing.generated.h"

class FJsonObject;
class FJsonValue;
enum class EThirdwebMarketplaceListingStatus : uint8;

USTRUCT(BlueprintType)
struct FThirdwebAssetCurrencyValue
{
	GENERATED_BODY()
    
	UPROPERTY(BlueprintReadWrite, Category="Currency Value")
	FString Name;

	UPROPERTY(BlueprintReadWrite, Category="Currency Value")
	FString Symbol;

	UPROPERTY(BlueprintReadWrite, Category="Currency Value")
	int32 Decimals = 0;

	UPROPERTY(BlueprintReadWrite, Category="Currency Value")
	FString Value;

	UPROPERTY(BlueprintReadWrite, Category="Currency Value")
	FString DisplayValue;

	static FThirdwebAssetCurrencyValue FromJson(const TSharedPtr<FJsonObject>& JsonObject);
};


USTRUCT(BlueprintType)
struct THIRDWEB_API FThirdwebAssetAttribute
{
	GENERATED_BODY()
    
	UPROPERTY(BlueprintReadWrite, Category="Asset Attribute")
	FString TraitType;

	UPROPERTY(BlueprintReadWrite, Category="Asset Attribute")
	FString Value;

	static FThirdwebAssetAttribute FromJson(const TSharedPtr<FJsonObject>& JsonObject);
	static TArray<FThirdwebAssetAttribute> FromJson(const TArray<TSharedPtr<FJsonValue>>& JsonArray);
};


USTRUCT(BlueprintType)
struct THIRDWEB_API FThirdwebAsset
{
	GENERATED_BODY()
    
	UPROPERTY(BlueprintReadWrite, Category="Asset")
	FString Id;

	UPROPERTY(BlueprintReadWrite, Category="Asset")
	FString Uri;

	UPROPERTY(BlueprintReadWrite, Category="Asset")
	FString Name;

	UPROPERTY(BlueprintReadWrite, Category="Asset")
	FString Description;

	UPROPERTY(BlueprintReadWrite, Category="Asset")
	FString ExternalUrl;

	UPROPERTY(BlueprintReadWrite, Category="Asset")
	TArray<FThirdwebAssetAttribute> Attributes;

	static FThirdwebAsset FromJson(const TSharedPtr<FJsonObject>& JsonObject);
};

USTRUCT(BlueprintType)
struct THIRDWEB_API FThirdwebMarketplaceListing
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category="Listing")
	FString AssetContractAddress;

	UPROPERTY(BlueprintReadWrite, Category="Listing")
	FString TokenId;

	UPROPERTY(BlueprintReadWrite, Category="Listing")
	FString CurrencyContractAddress;

	UPROPERTY(BlueprintReadWrite, Category="Listing")
	FString Quantity;

	UPROPERTY(BlueprintReadWrite, Category="Listing")
	FString PricePerToken;

	UPROPERTY(BlueprintReadWrite, Category="Listing")
	bool bIsReservedListing = false;

	UPROPERTY(BlueprintReadWrite, Category="Listing")
	FString Id;

	UPROPERTY(BlueprintReadWrite, Category="Listing")
	FThirdwebAssetCurrencyValue CurrencyValuePerToken;

	UPROPERTY(BlueprintReadWrite, Category="Listing")
	FThirdwebAsset Asset;

	UPROPERTY(BlueprintReadWrite, Category="Listing")
	EThirdwebMarketplaceListingStatus Status;

	UPROPERTY(BlueprintReadWrite, Category="Listing")
	FDateTime StartTimeInSeconds;

	UPROPERTY(BlueprintReadWrite, Category="Listing")
	FDateTime EndTimeInSeconds;

	static FThirdwebMarketplaceListing FromJson(const TSharedPtr<FJsonObject>& JsonObject);
	static TArray<FThirdwebMarketplaceListing> FromJson(const TArray<TSharedPtr<FJsonValue>>& JsonArray);
};