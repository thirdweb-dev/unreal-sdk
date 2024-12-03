// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "AsyncTasks/Engine/Marketplace/AsyncTaskThirdwebEngineMarketplaceBase.h"
#include "AsyncTaskThirdwebEngineMarketplaceEnglishAuctionsGet.generated.h"

class UThirdwebMarketplace;
struct FThirdwebMarketplaceEnglishAuction;
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class THIRDWEB_API UAsyncTaskThirdwebEngineMarketplaceEnglishAuctionsGet : public UAsyncTaskThirdwebEngineMarketplaceBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, DisplayName="Get English Auction", meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject"),
		Category="Thirdweb|Engine|Marketplace|English Auctions|Read|Raw")
	static UAsyncTaskThirdwebEngineMarketplaceEnglishAuctionsGet* Get(
		UObject* WorldContextObject,
		UPARAM(DisplayName="Chain ID") const int64 ChainId,
		const FString& Contract,
		UPARAM(DisplayName="Listing ID") const FString& ListingId
	);

	UFUNCTION(BlueprintCallable, DisplayName="Get English Auction", meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject"),
		Category="Thirdweb|Engine|Marketplace|English Auctions|Read")
	static UAsyncTaskThirdwebEngineMarketplaceEnglishAuctionsGet* MarketplaceGet(
		UObject* WorldContextObject,
		const UThirdwebMarketplace* Marketplace,
		UPARAM(DisplayName="Listing ID") const FString& ListingId
	);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGetEnglishAuctionDelegate, const FThirdwebMarketplaceEnglishAuction&, Listing, const FString&, Error);

	UPROPERTY(BlueprintAssignable)
	FGetEnglishAuctionDelegate Success;

	UPROPERTY(BlueprintAssignable)
	FGetEnglishAuctionDelegate Failed;

protected:
	UPROPERTY(Transient)
	FString ListingId;

private:
	virtual void HandleResponse(const FThirdwebMarketplaceEnglishAuction& Listing);
	void HandleFailed(const FString& Error);

public:
	virtual void Activate() override;
};
