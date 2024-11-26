// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "AsyncTasks/Engine/Marketplace/AsyncTaskThirdwebEngineMarketplaceBase.h"
#include "AsyncTaskThirdwebEngineMarketplaceEnglishAuctionsGetWinningBid.generated.h"

struct FThirdwebMarketplaceBid;
class UThirdwebMarketplace;
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class THIRDWEB_API UAsyncTaskThirdwebEngineMarketplaceEnglishAuctionsGetWinningBid : public UAsyncTaskThirdwebEngineMarketplaceBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, DisplayName="Get Winning Bid for English Auction", meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject"),
		Category="Thirdweb|Engine|Marketplace|English Auctions|Read|Raw")
	static UAsyncTaskThirdwebEngineMarketplaceEnglishAuctionsGetWinningBid* GetWinningBid(
		UObject* WorldContextObject,
		UPARAM(DisplayName="Chain ID") const int64 ChainId,
		const FString& Contract,
		UPARAM(DisplayName="Listing ID") const FString& ListingId
	);

	UFUNCTION(BlueprintCallable, DisplayName="Get Winning Bid for English Auction", meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject"),
		Category="Thirdweb|Engine|Marketplace|English Auctions|Read")
	static UAsyncTaskThirdwebEngineMarketplaceEnglishAuctionsGetWinningBid* MarketplaceGetWinningBid(
		UObject* WorldContextObject,
		const UThirdwebMarketplace* Marketplace,
		UPARAM(DisplayName="Listing ID") const FString& ListingId
	);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGetWinningBidDelegate, const  FThirdwebMarketplaceBid&, WinningBid, const FString&, Error);

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
