// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "AsyncTasks/Engine/Marketplace/AsyncTaskThirdwebEngineMarketplaceBase.h"
#include "AsyncTaskThirdwebEngineMarketplaceEnglishAuctionsGetBidBufferBps.generated.h"

class UThirdwebMarketplace;
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class THIRDWEB_API UAsyncTaskThirdwebEngineMarketplaceEnglishAuctionsGetBidBufferBps : public UAsyncTaskThirdwebEngineMarketplaceBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, DisplayName="Get Bid Buffer BPS for English Auction", meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject"),
		Category="Thirdweb|Engine|Marketplace|English Auctions|Read|Raw")
	static UAsyncTaskThirdwebEngineMarketplaceEnglishAuctionsGetBidBufferBps* GetBidBufferBps(
		UObject* WorldContextObject,
		UPARAM(DisplayName="Chain ID") const int64 ChainId,
		const FString& Contract,
		UPARAM(DisplayName="Listing ID") const FString& ListingId
	);

	UFUNCTION(BlueprintCallable, DisplayName="Get Bid Buffer BPS for English Auction", meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject"),
		Category="Thirdweb|Engine|Marketplace|English Auctions|Read")
	static UAsyncTaskThirdwebEngineMarketplaceEnglishAuctionsGetBidBufferBps* MarketplaceGetBidBufferBps(
		UObject* WorldContextObject,
		const UThirdwebMarketplace* Marketplace,
		UPARAM(DisplayName="Listing ID") const FString& ListingId
	);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGetBidBufferBpsDelegate, const int32, BPS, const FString&, Error);

	UPROPERTY(BlueprintAssignable)
	FGetBidBufferBpsDelegate Success;

	UPROPERTY(BlueprintAssignable)
	FGetBidBufferBpsDelegate Failed;

protected:
	UPROPERTY(Transient)
	FString ListingId;

private:
	virtual void HandleResponse(const int32 Bps);
	void HandleFailed(const FString& Error);

public:
	virtual void Activate() override;
};
