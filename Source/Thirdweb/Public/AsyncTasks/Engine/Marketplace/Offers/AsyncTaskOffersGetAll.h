// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "AsyncTasks/Engine/Marketplace/AsyncTaskMarketplaceGetBase.h"
#include "AsyncTaskOffersGetAll.generated.h"

struct FThirdwebMarketplaceOffer;
class UThirdwebMarketplace;

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class THIRDWEB_API UAsyncTaskOffersGetAll : public UAsyncTaskMarketplaceGetBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, DisplayName="Get All Offers",
		meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject", AdvancedDisplay="Count,Offeror,Start,TokenId,TokenAddress"),
		Category="Thirdweb|Engine|Marketplace|Offers|Read|Raw")
	static UAsyncTaskOffersGetAll* GetAll(
		UObject* WorldContextObject,
		UPARAM(DisplayName="Chain ID") const int64 ChainId,
		const FString& Contract,
		const bool bOnlyValid,
		const int32 Count,
		const int32 Start,
		const FString& Offeror,
		const FString& TokenAddress,
		UPARAM(DisplayName="Token ID") const FString& TokenId
	);

	UFUNCTION(BlueprintCallable, DisplayName="Get All Offers",
		meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject", AdvancedDisplay="Count,Offeror,Start,TokenId,TokenAddress"),
		Category="Thirdweb|Engine|Marketplace|Offers|Read")
	static UAsyncTaskOffersGetAll* MarketplaceGetAll(
		UObject* WorldContextObject,
		const UThirdwebMarketplace* Marketplace,
		const bool bOnlyValid,
		const int32 Count,
		const int32 Start,
		const FString& Offeror,
		const FString& TokenAddress,
		UPARAM(DisplayName="Token ID") const FString& TokenId
	);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGetAllOffersDelegate, const TArray<FThirdwebMarketplaceOffer>&, Listings, const FString&, Error);

	UPROPERTY(BlueprintAssignable)
	FGetAllOffersDelegate Success;

	UPROPERTY(BlueprintAssignable)
	FGetAllOffersDelegate Failed;

protected:
	UPROPERTY(Transient)
	int32 Count;

	UPROPERTY(Transient)
	FString Offeror;

	UPROPERTY(Transient)
	int32 Start;

	UPROPERTY(Transient)
	FString TokenContract;

	UPROPERTY(Transient)
	FString TokenId;

	UPROPERTY(Transient)
	bool bOnlyValid;

private:
	virtual void HandleResponse(const TArray<FThirdwebMarketplaceOffer>& Offers);
	void HandleFailed(const FString& Error);

public:
	virtual void Activate() override;
};
