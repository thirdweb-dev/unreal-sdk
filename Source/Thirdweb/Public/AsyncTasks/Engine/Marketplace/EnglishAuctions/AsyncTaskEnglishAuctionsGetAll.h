// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "AsyncTasks/Engine/Marketplace/AsyncTaskMarketplaceGetBase.h"
#include "AsyncTaskEnglishAuctionsGetAll.generated.h"

struct FThirdwebMarketplaceEnglishAuction;
class UThirdwebMarketplace;

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class THIRDWEB_API UAsyncTaskEnglishAuctionsGetAll : public UAsyncTaskMarketplaceGetBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, DisplayName="Get All English Auctions",
		meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject", AdvancedDisplay="Count,Seller,Start,TokenId,TokenAddress"),
		Category="Thirdweb|Engine|Marketplace|English Auctions|Read|Raw")
	static UAsyncTaskEnglishAuctionsGetAll* GetAll(
		UObject* WorldContextObject,
		UPARAM(DisplayName="Chain ID") const int64 ChainId,
		const FString& Contract,
		const bool bOnlyValid,
		const int32 Count,
		const int32 Start,
		const FString& Seller,
		const FString& TokenAddress,
		UPARAM(DisplayName="Token ID") const FString& TokenId
	);

	UFUNCTION(BlueprintCallable, DisplayName="Get All English Auctions",
		meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject", AdvancedDisplay="Count,Seller,Start,TokenId,TokenAddress"),
		Category="Thirdweb|Engine|Marketplace|English Auctions|Read")
	static UAsyncTaskEnglishAuctionsGetAll* MarketplaceGetAll(
		UObject* WorldContextObject,
		const UThirdwebMarketplace* Marketplace,
		const bool bOnlyValid,
		const int32 Count,
		const int32 Start,
		const FString& Seller,
		const FString& TokenAddress,
		UPARAM(DisplayName="Token ID") const FString& TokenId
	);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGetAllEnglishAuctionsDelegate, const TArray<FThirdwebMarketplaceEnglishAuction>&, Listings, const FString&, Error);

	UPROPERTY(BlueprintAssignable)
	FGetAllEnglishAuctionsDelegate Success;

	UPROPERTY(BlueprintAssignable)
	FGetAllEnglishAuctionsDelegate Failed;

protected:
	UPROPERTY(Transient)
	int32 Count;

	UPROPERTY(Transient)
	FString Seller;

	UPROPERTY(Transient)
	int32 Start;

	UPROPERTY(Transient)
	FString TokenContract;

	UPROPERTY(Transient)
	FString TokenId;

	UPROPERTY(Transient)
	bool bOnlyValid;

private:
	virtual void HandleResponse(const TArray<FThirdwebMarketplaceEnglishAuction>& Listings);
	void HandleFailed(const FString& Error);

public:
	virtual void Activate() override;
};
