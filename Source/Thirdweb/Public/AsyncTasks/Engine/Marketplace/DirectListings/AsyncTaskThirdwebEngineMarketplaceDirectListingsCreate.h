// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "AsyncTasks/Engine/Marketplace/AsyncTaskThirdwebEngineMarketplacePostBase.h"
#include "Engine/Marketplace/DirectListings/ThirdwebMarketplaceListing.h"
#include "AsyncTaskThirdwebEngineMarketplaceDirectListingsCreate.generated.h"

class UThirdwebMarketplace;
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class THIRDWEB_API UAsyncTaskThirdwebEngineMarketplaceDirectListingsCreate : public UAsyncTaskThirdwebEngineMarketplacePostBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, DisplayName="Create Direct Listing",
		meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject", AdvancedDisplay="IdempotencyKey,bSimulateTx"),
		Category="Thirdweb|Engine|Marketplace|Direct Listings|Write|Raw")
	static UAsyncTaskThirdwebEngineMarketplaceDirectListingsCreate* Create(
		UObject* WorldContextObject,
		UPARAM(DisplayName="Chain ID") const int64 ChainId,
		const FString& Contract,
		const FString& BackendWallet,
		const FSmartWalletHandle& Account,
		const FThirdwebMarketplaceCreateListingRequest& Listing,
		const FString& IdempotencyKey,
		UPARAM(DisplayName="Simulate TX") const bool bSimulateTx
	);

	UFUNCTION(BlueprintCallable, DisplayName="Create Direct Listing",
		meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject", AdvancedDisplay="IdempotencyKey,bSimulateTx"),
		Category="Thirdweb|Engine|Marketplace|Direct Listings|Write")
	static UAsyncTaskThirdwebEngineMarketplaceDirectListingsCreate* MarketplaceCreate(
		UObject* WorldContextObject,
		const UThirdwebMarketplace* Marketplace,
		const FString& BackendWallet,
		const FSmartWalletHandle& Account,
		const FThirdwebMarketplaceCreateListingRequest& Listing,
		const FString& IdempotencyKey,
		UPARAM(DisplayName="Simulate TX") const bool bSimulateTx
	);

protected:
	UPROPERTY(Transient)
	FThirdwebMarketplaceCreateListingRequest Listing;

public:
	virtual void Activate() override;
};
