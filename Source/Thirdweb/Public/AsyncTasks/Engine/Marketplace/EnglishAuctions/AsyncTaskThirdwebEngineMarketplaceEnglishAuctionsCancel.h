// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "AsyncTasks/Engine/Marketplace/AsyncTaskThirdwebEngineMarketplacePostBase.h"
#include "AsyncTaskThirdwebEngineMarketplaceEnglishAuctionsCancel.generated.h"

class UThirdwebMarketplace;
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class THIRDWEB_API UAsyncTaskThirdwebEngineMarketplaceEnglishAuctionsCancel : public UAsyncTaskThirdwebEngineMarketplacePostBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, DisplayName="Cancel English Auction",
		meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject", AdvancedDisplay="IdempotencyKey,bSimulateTx"),
		Category="Thirdweb|Engine|Marketplace|English Auctions|Write|Raw")
	static UAsyncTaskThirdwebEngineMarketplaceEnglishAuctionsCancel* Cancel(
		UObject* WorldContextObject,
		UPARAM(DisplayName="Chain ID") const int64 ChainId,
		const FString& Contract,
		const FString& BackendWallet,
		const FSmartWalletHandle& Account,
		UPARAM(DisplayName="Listing ID") const FString& ListingId,
		const FString& IdempotencyKey,
		UPARAM(DisplayName="Simulate TX") const bool bSimulateTx
	);

	UFUNCTION(BlueprintCallable, DisplayName="Cancel English Auction",
		meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject", AdvancedDisplay="IdempotencyKey,bSimulateTx"),
		Category="Thirdweb|Engine|Marketplace|English Auctions|Write")
	static UAsyncTaskThirdwebEngineMarketplaceEnglishAuctionsCancel* MarketplaceCancel(
		UObject* WorldContextObject,
		const UThirdwebMarketplace* Marketplace,
		const FString& BackendWallet,
		const FSmartWalletHandle& Account,
		UPARAM(DisplayName="Listing ID") const FString& ListingId,
		const FString& IdempotencyKey,
		UPARAM(DisplayName="Simulate TX") const bool bSimulateTx
	);

protected:
	UPROPERTY(Transient)
	FString ListingId;

public:
	virtual void Activate() override;
};
