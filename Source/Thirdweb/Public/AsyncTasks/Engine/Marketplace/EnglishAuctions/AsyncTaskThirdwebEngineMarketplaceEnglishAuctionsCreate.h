// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "AsyncTasks/Engine/Marketplace/AsyncTaskThirdwebEngineMarketplacePostBase.h"
#include "Engine/Marketplace/EnglishAuctions/ThirdwebMarketplaceCreateEnglishAuctionRequest.h"
#include "AsyncTaskThirdwebEngineMarketplaceEnglishAuctionsCreate.generated.h"

class UThirdwebMarketplace;
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class THIRDWEB_API UAsyncTaskThirdwebEngineMarketplaceEnglishAuctionsCreate : public UAsyncTaskThirdwebEngineMarketplacePostBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, DisplayName="Create an English Auction",
		meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject", AdvancedDisplay="IdempotencyKey,bSimulateTx"),
		Category="Thirdweb|Engine|Marketplace|English Auctions|Write|Raw")
	static UAsyncTaskThirdwebEngineMarketplaceEnglishAuctionsCreate* Create(
		UObject* WorldContextObject,
		UPARAM(DisplayName="Chain ID") const int64 ChainId,
		const FString& Contract,
		const FString& BackendWallet,
		const FSmartWalletHandle& Account,
		const FThirdwebMarketplaceCreateEnglishAuctionRequest& Auction,
		const FString& IdempotencyKey,
		UPARAM(DisplayName="Simulate TX") const bool bSimulateTx
	);

	UFUNCTION(BlueprintCallable, DisplayName="Create an English Auction",
		meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject", AdvancedDisplay="IdempotencyKey,bSimulateTx"),
		Category="Thirdweb|Engine|Marketplace|English Auctions|Write")
	static UAsyncTaskThirdwebEngineMarketplaceEnglishAuctionsCreate* MarketplaceCreate(
		UObject* WorldContextObject,
		const UThirdwebMarketplace* Marketplace,
		const FString& BackendWallet,
		const FSmartWalletHandle& Account,
		const FThirdwebMarketplaceCreateEnglishAuctionRequest& Auction,
		const FString& IdempotencyKey,
		UPARAM(DisplayName="Simulate TX") const bool bSimulateTx
	);

protected:
	UPROPERTY(Transient)
	FThirdwebMarketplaceCreateEnglishAuctionRequest Auction;

	
public:
	virtual void Activate() override;
};
