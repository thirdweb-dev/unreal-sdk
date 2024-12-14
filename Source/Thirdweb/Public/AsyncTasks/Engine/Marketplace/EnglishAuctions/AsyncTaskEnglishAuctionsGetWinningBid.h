// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "AsyncTasks/Engine/Marketplace/AsyncTaskMarketplaceGetBase.h"
#include "Engine/Marketplace/EnglishAuctions/ThirdwebMarketplaceBid.h"
#include "AsyncTaskEnglishAuctionsGetWinningBid.generated.h"

class UThirdwebMarketplace;
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class THIRDWEB_API UAsyncTaskEnglishAuctionsGetWinningBid : public UAsyncTaskMarketplaceGetBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, DisplayName="Get Winning Bid for English Auction", meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject"),
		Category="Thirdweb|Engine|Marketplace|English Auctions|Read|Raw")
	static UAsyncTaskEnglishAuctionsGetWinningBid* GetWinningBid(
		UObject* WorldContextObject,
		UPARAM(DisplayName="Chain ID") const int64 ChainId,
		const FString& Contract,
		UPARAM(DisplayName="Listing ID") const FString& ListingId
	);

	UFUNCTION(BlueprintCallable, DisplayName="Get Winning Bid for English Auction", meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject"),
		Category="Thirdweb|Engine|Marketplace|English Auctions|Read")
	static UAsyncTaskEnglishAuctionsGetWinningBid* MarketplaceGetWinningBid(
		UObject* WorldContextObject,
		const UThirdwebMarketplace* Marketplace,
		UPARAM(DisplayName="Listing ID") const FString& ListingId
	);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGetWinningBidDelegate, const FThirdwebMarketplaceBid&, WinningBid, const FString&, Error);

	UPROPERTY(BlueprintAssignable)
	FGetWinningBidDelegate Success;

	UPROPERTY(BlueprintAssignable)
	FGetWinningBidDelegate Failed;

protected:
	UPROPERTY(Transient)
	FString ListingId;

private:
	virtual void HandleResponse(const FThirdwebMarketplaceBid& WinningBid);
	void HandleFailed(const FString& Error);

public:
	virtual void Activate() override;
};
