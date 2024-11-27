// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "AsyncTasks/Engine/Marketplace/AsyncTaskThirdwebEngineMarketplaceBase.h"
#include "AsyncTaskThirdwebEngineMarketplaceDirectListingsGet.generated.h"

class UThirdwebMarketplace;
struct FThirdwebMarketplaceDirectListing;
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class THIRDWEB_API UAsyncTaskThirdwebEngineMarketplaceDirectListingsGet : public UAsyncTaskThirdwebEngineMarketplaceBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, DisplayName="Get Direct Listing", meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject"),
		Category="Thirdweb|Engine|Marketplace|Direct Listings|Read|Raw")
	static UAsyncTaskThirdwebEngineMarketplaceDirectListingsGet* Get(
		UObject* WorldContextObject,
		UPARAM(DisplayName="Chain ID") const int64 ChainId,
		const FString& Contract,
		UPARAM(DisplayName="Listing ID") const FString& ListingId
	);

	UFUNCTION(BlueprintCallable, DisplayName="Get Direct Listing", meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject"),
		Category="Thirdweb|Engine|Marketplace|Direct Listings|Read")
	static UAsyncTaskThirdwebEngineMarketplaceDirectListingsGet* MarketplaceGet(
		UObject* WorldContextObject,
		const UThirdwebMarketplace* Marketplace,
		UPARAM(DisplayName="Listing ID") const FString& ListingId
	);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGetDirectListingDelegate, const FThirdwebMarketplaceDirectListing&, Listing, const FString&, Error);

	UPROPERTY(BlueprintAssignable)
	FGetDirectListingDelegate Success;

	UPROPERTY(BlueprintAssignable)
	FGetDirectListingDelegate Failed;

protected:
	UPROPERTY(Transient)
	FString ListingId;

private:
	virtual void HandleResponse(const FThirdwebMarketplaceDirectListing& Listing);
	void HandleFailed(const FString& Error);

public:
	virtual void Activate() override;
};
