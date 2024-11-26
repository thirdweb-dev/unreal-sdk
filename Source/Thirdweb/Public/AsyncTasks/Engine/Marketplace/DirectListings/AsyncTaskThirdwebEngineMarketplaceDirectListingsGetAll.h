// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "AsyncTasks/AsyncTaskThirdwebBase.h"
#include "AsyncTaskThirdwebEngineMarketplaceDirectListingsGetAll.generated.h"

struct FThirdwebMarketplaceListing;
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class THIRDWEB_API UAsyncTaskThirdwebEngineMarketplaceDirectListingsGetAll : public UAsyncTaskThirdwebBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, DisplayName="Create SIWE Wallet", meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject"), Category="Thirdweb|Engine|Marketplace|Direct Listings")
	static UAsyncTaskThirdwebEngineMarketplaceDirectListingsGetAll* GetAllDirectListings(
		UObject* WorldContextObject,
		const int32 Count,
		const FString& Seller,
		const int32 Start,
		const FString& TokenContract,
		const FString& TokenId,
		const int64 Chain,
		const FString& ContractAddress
	);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGetAllDirectListingsDelegate, const TArray<FThirdwebMarketplaceListing>&, Result, const FString&, Error);

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
	int64 Chain;
	
	UPROPERTY(Transient)
	FString ContractAddress;

private:
	virtual void HandleResponse(const TArray<FThirdwebMarketplaceListing>& Listings);
	void HandleFailed(const FString& Error);

public:
	virtual void Activate() override;
};
