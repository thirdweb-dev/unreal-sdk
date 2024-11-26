// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "AsyncTasks/Engine/Marketplace/AsyncTaskThirdwebEngineMarketplacePostBase.h"
#include "Engine/Marketplace/DirectListings/ThirdwebMarketplaceListing.h"
#include "AsyncTaskThirdwebEngineMarketplaceDirectListingsRevokeReservedCurrencyApproval.generated.h"

class UThirdwebMarketplace;
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class THIRDWEB_API UAsyncTaskThirdwebEngineMarketplaceDirectListingsRevokeReservedCurrencyApproval : public UAsyncTaskThirdwebEngineMarketplacePostBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, DisplayName="Revoke Currency Approval for Reserved Direct Listing",
		meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject", AdvancedDisplay="IdempotencyKey,bSimulateTx"),
		Category="Thirdweb|Engine|Marketplace|Direct Listings|Write|Raw")
	static UAsyncTaskThirdwebEngineMarketplaceDirectListingsRevokeReservedCurrencyApproval* RevokeReservedCurrencyApproval(
		UObject* WorldContextObject,
		UPARAM(DisplayName="Chain ID") const int64 ChainId,
		const FString& Contract,
		const FString& BackendWallet,
		const FSmartWalletHandle& Account,
		UPARAM(DisplayName="Listing ID") const FString& ListingId,
		const FString& Currency,
		const FString& IdempotencyKey,
		UPARAM(DisplayName="Simulate TX") const bool bSimulateTx
	);

	UFUNCTION(BlueprintCallable, DisplayName="Revoke Currency Approval for Reserved Direct Listing",
		meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject", AdvancedDisplay="IdempotencyKey,bSimulateTx"),
		Category="Thirdweb|Engine|Marketplace|Direct Listings|Write")
	static UAsyncTaskThirdwebEngineMarketplaceDirectListingsRevokeReservedCurrencyApproval* MarketplaceRevokeReservedCurrencyApproval(
		UObject* WorldContextObject,
		const UThirdwebMarketplace* Marketplace,
		const FString& BackendWallet,
		const FSmartWalletHandle& Account,
		UPARAM(DisplayName="Listing ID") const FString& ListingId,
		const FString& Currency,
		const FString& IdempotencyKey,
		UPARAM(DisplayName="Simulate TX") const bool bSimulateTx
	);

protected:
	UPROPERTY(Transient)
	FString ListingId;

	UPROPERTY(Transient)
	FString Currency;

public:
	virtual void Activate() override;
};
