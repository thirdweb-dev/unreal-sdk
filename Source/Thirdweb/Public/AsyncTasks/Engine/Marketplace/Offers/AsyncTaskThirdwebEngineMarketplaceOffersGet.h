// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "AsyncTasks/Engine/Marketplace/AsyncTaskThirdwebEngineMarketplaceBase.h"
#include "AsyncTaskThirdwebEngineMarketplaceOffersGet.generated.h"

struct FThirdwebMarketplaceOffer;
class UThirdwebMarketplace;

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class THIRDWEB_API UAsyncTaskThirdwebEngineMarketplaceOffersGet : public UAsyncTaskThirdwebEngineMarketplaceBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, DisplayName="Get Offer",
		meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject", AdvancedDisplay="Count,Offeror,Start,TokenId,TokenAddress"),
		Category="Thirdweb|Engine|Marketplace|Offers|Read|Raw")
	static UAsyncTaskThirdwebEngineMarketplaceOffersGet* Get(
		UObject* WorldContextObject,
		UPARAM(DisplayName="Chain ID") const int64 ChainId,
		const FString& Contract,
		UPARAM(DisplayName="Offer ID") const FString& OfferId
	);

	UFUNCTION(BlueprintCallable, DisplayName="Get Offer",
		meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject", AdvancedDisplay="Count,Offeror,Start,TokenId,TokenAddress"),
		Category="Thirdweb|Engine|Marketplace|Offers|Read")
	static UAsyncTaskThirdwebEngineMarketplaceOffersGet* MarketplaceGet(
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
