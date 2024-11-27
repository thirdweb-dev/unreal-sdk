// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "AsyncTasks/Engine/Marketplace/AsyncTaskThirdwebEngineMarketplaceBase.h"
#include "AsyncTaskThirdwebEngineMarketplaceDirectListingsGetAll.generated.h"

class UThirdwebMarketplace;
struct FThirdwebMarketplaceDirectListing;
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class THIRDWEB_API UAsyncTaskThirdwebEngineMarketplaceDirectListingsGetAll : public UAsyncTaskThirdwebEngineMarketplaceBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, DisplayName="Get All Direct Listings",
		meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject", AdvancedDisplay="Count,Seller,Start,TokenId,TokenAddress"),
		Category="Thirdweb|Engine|Marketplace|Direct Listings|Read|Raw")
	static UAsyncTaskThirdwebEngineMarketplaceDirectListingsGetAll* GetAll(
		UObject* WorldContextObject,
		UPARAM(DisplayName="Chain ID") const int64 ChainId,
		const FString& Contract,
		const bool bOnlyValid,
		const int32 Count,
		const int32 Start,
		const FString& Seller,
		const FString& TokenAddress,
		UPARAM(DisplayName="Token ID") const FString& TokenId
	);

	UFUNCTION(BlueprintCallable, DisplayName="Get All Direct Listings",
		meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject", AdvancedDisplay="Count,Seller,Start,TokenId,TokenAddress"),
		Category="Thirdweb|Engine|Marketplace|Direct Listings|Read")
	static UAsyncTaskThirdwebEngineMarketplaceDirectListingsGetAll* MarketplaceGetAll(
		UObject* WorldContextObject,
		const UThirdwebMarketplace* Marketplace,
		const bool bOnlyValid,
		const int32 Count,
		const int32 Start,
		const FString& Seller,
		const FString& TokenAddress,
		UPARAM(DisplayName="Token ID") const FString& TokenId
	);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGetAllDirectListingsDelegate, const TArray<FThirdwebMarketplaceDirectListing>&, Listings, const FString&, Error);

	UPROPERTY(BlueprintAssignable)
	FGetAllDirectListingsDelegate Success;

	UPROPERTY(BlueprintAssignable)
	FGetAllDirectListingsDelegate Failed;

protected:
	UPROPERTY(Transient)
	int32 Count;

	UPROPERTY(Transient)
	FString Seller;

	UPROPERTY(Transient)
	int32 Start;

	UPROPERTY(Transient)
	FString TokenContract;

	UPROPERTY(Transient)
	FString TokenId;

	UPROPERTY(Transient)
	bool bOnlyValid;

private:
	virtual void HandleResponse(const TArray<FThirdwebMarketplaceDirectListing>& Listings);
	void HandleFailed(const FString& Error);

public:
	virtual void Activate() override;
};
