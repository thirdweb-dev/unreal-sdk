// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "AsyncTasks/Engine/Marketplace/AsyncTaskMarketplaceGetBase.h"
#include "AsyncTaskOffersGet.generated.h"

struct FThirdwebMarketplaceOffer;
class UThirdwebMarketplace;

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class THIRDWEB_API UAsyncTaskOffersGet : public UAsyncTaskMarketplaceGetBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, DisplayName="Get Offer",
		meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject", AdvancedDisplay="Count,Offeror,Start,TokenId,TokenAddress"),
		Category="Thirdweb|Engine|Marketplace|Offers|Read|Raw")
	static UAsyncTaskOffersGet* Get(
		UObject* WorldContextObject,
		UPARAM(DisplayName="Chain ID") const int64 ChainId,
		const FString& Contract,
		UPARAM(DisplayName="Offer ID") const FString& OfferId
	);

	UFUNCTION(BlueprintCallable, DisplayName="Get Offer",
		meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject", AdvancedDisplay="Count,Offeror,Start,TokenId,TokenAddress"),
		Category="Thirdweb|Engine|Marketplace|Offers|Read")
	static UAsyncTaskOffersGet* MarketplaceGet(
		UObject* WorldContextObject,
		const UThirdwebMarketplace* Marketplace,
		UPARAM(DisplayName="Offer ID") const FString& OfferId
	);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGetOffersDelegate, const FThirdwebMarketplaceOffer&, Listings, const FString&, Error);

	UPROPERTY(BlueprintAssignable)
	FGetOffersDelegate Success;

	UPROPERTY(BlueprintAssignable)
	FGetOffersDelegate Failed;

protected:
	UPROPERTY(Transient)
	FString OfferId;

private:
	virtual void HandleResponse(const FThirdwebMarketplaceOffer& Offer);
	void HandleFailed(const FString& Error);

public:
	virtual void Activate() override;
};
