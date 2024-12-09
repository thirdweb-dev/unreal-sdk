// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "AsyncTasks/Engine/Marketplace/AsyncTaskMarketplaceGetBase.h"
#include "AsyncTaskEnglishAuctionsGetMinimumNextBid.generated.h"

class UThirdwebMarketplace;
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class THIRDWEB_API UAsyncTaskEnglishAuctionsGetMinimumNextBid : public UAsyncTaskMarketplaceGetBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, DisplayName="Get Minimum Next Bid for English Auction", meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject"),
		Category="Thirdweb|Engine|Marketplace|English Auctions|Read|Raw")
	static UAsyncTaskEnglishAuctionsGetMinimumNextBid* GetMinimumNextBid(
		UObject* WorldContextObject,
		UPARAM(DisplayName="Chain ID") const int64 ChainId,
		const FString& Contract,
		UPARAM(DisplayName="Listing ID") const FString& ListingId
	);

	UFUNCTION(BlueprintCallable, DisplayName="Get Minimum Next Bid for English Auction", meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject"),
		Category="Thirdweb|Engine|Marketplace|English Auctions|Read")
	static UAsyncTaskEnglishAuctionsGetMinimumNextBid* MarketplaceGetMinimumNextBid(
		UObject* WorldContextObject,
		const UThirdwebMarketplace* Marketplace,
		UPARAM(DisplayName="Listing ID") const FString& ListingId
	);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGetMinimumNextBidDelegate, const FString&, MinimumNextBid, const FString&, Error);

	UPROPERTY(BlueprintAssignable)
	FGetMinimumNextBidDelegate Success;

	UPROPERTY(BlueprintAssignable)
	FGetMinimumNextBidDelegate Failed;

protected:
	UPROPERTY(Transient)
	FString ListingId;

private:
	virtual void HandleResponse(const FString& MinimumNextBid);
	void HandleFailed(const FString& Error);

public:
	virtual void Activate() override;
};
