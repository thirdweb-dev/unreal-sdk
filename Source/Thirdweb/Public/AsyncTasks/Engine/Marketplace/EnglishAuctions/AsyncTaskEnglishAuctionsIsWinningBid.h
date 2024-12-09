// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "AsyncTasks/Engine/Marketplace/AsyncTaskMarketplaceGetBase.h"
#include "AsyncTaskEnglishAuctionsIsWinningBid.generated.h"

class UThirdwebMarketplace;
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class THIRDWEB_API UAsyncTaskEnglishAuctionsIsWinningBid : public UAsyncTaskMarketplaceGetBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, DisplayName="Is Winning Bid for English Auction", meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject"),
		Category="Thirdweb|Engine|Marketplace|English Auctions|Read|Raw")
	static UAsyncTaskEnglishAuctionsIsWinningBid* IsWinningBid(
		UObject* WorldContextObject,
		UPARAM(DisplayName="Chain ID") const int64 ChainId,
		const FString& Contract,
		UPARAM(DisplayName="Listing ID") const FString& ListingId,
		const FString& BidAmount
	);

	UFUNCTION(BlueprintCallable, DisplayName="Is Winning Bid for English Auction", meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject"),
		Category="Thirdweb|Engine|Marketplace|English Auctions|Read")
	static UAsyncTaskEnglishAuctionsIsWinningBid* MarketplaceIsWinningBid(
		UObject* WorldContextObject,
		const UThirdwebMarketplace* Marketplace,
		UPARAM(DisplayName="Listing ID") const FString& ListingId,
		const FString& BidAmount
	);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FIsWinningBidDelegate, const bool, bIsWinningBid, const FString&, Error);

	UPROPERTY(BlueprintAssignable)
	FIsWinningBidDelegate Success;

	UPROPERTY(BlueprintAssignable)
	FIsWinningBidDelegate Failed;

protected:
	UPROPERTY(Transient)
	FString ListingId;

	UPROPERTY(Transient)
	FString BidAmount;

private:
	virtual void HandleResponse(const bool bIsWinningBid);
	void HandleFailed(const FString& Error);

public:
	virtual void Activate() override;
};
