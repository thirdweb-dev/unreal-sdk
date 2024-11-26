// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "AsyncTasks/Engine/Marketplace/AsyncTaskThirdwebEngineMarketplacePostBase.h"
#include "AsyncTaskThirdwebEngineMarketplaceOffersAccept.generated.h"

class UThirdwebMarketplace;
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class THIRDWEB_API UAsyncTaskThirdwebEngineMarketplaceOffersAccept : public UAsyncTaskThirdwebEngineMarketplacePostBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, DisplayName="Accept Offer",
		meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject", AdvancedDisplay="IdempotencyKey,bSimulateTx"),
		Category="Thirdweb|Engine|Marketplace|Offers|Write|Raw")
	static UAsyncTaskThirdwebEngineMarketplaceOffersAccept* Accept(
		UObject* WorldContextObject,
		UPARAM(DisplayName="Chain ID") const int64 ChainId,
		const FString& Contract,
		const FString& BackendWallet,
		const FSmartWalletHandle& Account,
		UPARAM(DisplayName="Offer ID") const FString& OfferId,
		const FString& IdempotencyKey,
		UPARAM(DisplayName="Simulate TX") const bool bSimulateTx
	);

	UFUNCTION(BlueprintCallable, DisplayName="Accept Offer",
		meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject", AdvancedDisplay="IdempotencyKey,bSimulateTx"),
		Category="Thirdweb|Engine|Marketplace|Offers|Write")
	static UAsyncTaskThirdwebEngineMarketplaceOffersAccept* MarketplaceAccept(
		UObject* WorldContextObject,
		const UThirdwebMarketplace* Marketplace,
		const FString& BackendWallet,
		const FSmartWalletHandle& Account,
		UPARAM(DisplayName="Offer ID") const FString& OfferId,
		const FString& IdempotencyKey,
		UPARAM(DisplayName="Simulate TX") const bool bSimulateTx
	);

protected:
	UPROPERTY(Transient)
	FString OfferId;
	
public:
	virtual void Activate() override;
};
