// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "AsyncTasks/Engine/Marketplace/AsyncTaskThirdwebEngineMarketplaceBase.h"
#include "AsyncTaskThirdwebEngineMarketplaceEnglishAuctionsGetWinner.generated.h"

class UThirdwebMarketplace;
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class THIRDWEB_API UAsyncTaskThirdwebEngineMarketplaceEnglishAuctionsGetWinner : public UAsyncTaskThirdwebEngineMarketplaceBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, DisplayName="Get Winner of English Auction", meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject"),
		Category="Thirdweb|Engine|Marketplace|English Auctions|Read|Raw")
	static UAsyncTaskThirdwebEngineMarketplaceEnglishAuctionsGetWinner* GetWinner(
		UObject* WorldContextObject,
		UPARAM(DisplayName="Chain ID") const int64 ChainId,
		const FString& Contract,
		UPARAM(DisplayName="Listing ID") const FString& ListingId
	);

	UFUNCTION(BlueprintCallable, DisplayName="Get Winner of English Auction", meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject"),
		Category="Thirdweb|Engine|Marketplace|English Auctions|Read")
	static UAsyncTaskThirdwebEngineMarketplaceEnglishAuctionsGetWinner* MarketplaceGetWinner(
		UObject* WorldContextObject,
		const UThirdwebMarketplace* Marketplace,
		UPARAM(DisplayName="Listing ID") const FString& ListingId
	);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGetWinnerDelegate, const FString&, Winner, const FString&, Error);

	UPROPERTY(BlueprintAssignable)
	FGetWinnerDelegate Success;

	UPROPERTY(BlueprintAssignable)
	FGetWinnerDelegate Failed;

protected:
	UPROPERTY(Transient)
	FString ListingId;

private:
	virtual void HandleResponse(const FString& Winner);
	void HandleFailed(const FString& Error);

public:
	virtual void Activate() override;
};
