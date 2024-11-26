// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "AsyncTasks/Engine/Marketplace/AsyncTaskThirdwebEngineMarketplaceBase.h"
#include "AsyncTaskThirdwebEngineMarketplaceDirectListingsIsBuyerApproved.generated.h"

class UThirdwebMarketplace;
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class THIRDWEB_API UAsyncTaskThirdwebEngineMarketplaceDirectListingsIsBuyerApproved : public UAsyncTaskThirdwebEngineMarketplaceBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, DisplayName="Is Buyer Approved For Direct Listing", meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject"),
		Category="Thirdweb|Engine|Marketplace|Direct Listings|Read|Raw")
	static UAsyncTaskThirdwebEngineMarketplaceDirectListingsIsBuyerApproved* IsBuyerApproved(
		UObject* WorldContextObject,
		UPARAM(DisplayName="Chain ID") const int64 ChainId,
		const FString& Contract,
		UPARAM(DisplayName="Listing ID") const FString& ListingId,
		const FString& Buyer
	);

	UFUNCTION(BlueprintCallable, DisplayName="Is Buyer Approved For Direct Listing", meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject"),
		Category="Thirdweb|Engine|Marketplace|Direct Listings|Read")
	static UAsyncTaskThirdwebEngineMarketplaceDirectListingsIsBuyerApproved* MarketplaceIsBuyerApproved(
		UObject* WorldContextObject,
		const UThirdwebMarketplace* Marketplace,
		UPARAM(DisplayName="Listing ID") const FString& ListingId,
		const FString& Buyer
	);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FIsBuyerApprovedDelegate, const bool, bApproved, const FString&, Error);

	UPROPERTY(BlueprintAssignable)
	FIsBuyerApprovedDelegate Success;

	UPROPERTY(BlueprintAssignable)
	FIsBuyerApprovedDelegate Failed;

protected:
	UPROPERTY(Transient)
	FString ListingId;

	UPROPERTY(Transient)
	FString WalletAddress;

private:
	virtual void HandleResponse(const bool bApproved);
	void HandleFailed(const FString& Error);

public:
	virtual void Activate() override;
};
