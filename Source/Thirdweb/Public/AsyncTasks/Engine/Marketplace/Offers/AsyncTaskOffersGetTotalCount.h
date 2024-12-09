// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "AsyncTasks/Engine/Marketplace/AsyncTaskMarketplaceGetBase.h"
#include "AsyncTaskOffersGetTotalCount.generated.h"

class UThirdwebMarketplace;
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class THIRDWEB_API UAsyncTaskOffersGetTotalCount : public UAsyncTaskMarketplaceGetBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, DisplayName="Get Total Count of Offers", meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject"),
		Category="Thirdweb|Engine|Marketplace|Offers|Read|Raw")
	static UAsyncTaskOffersGetTotalCount* GetTotalCount(
		UObject* WorldContextObject,
		UPARAM(DisplayName="Chain ID") const int64 ChainId,
		const FString& Contract
	);

	UFUNCTION(BlueprintCallable, DisplayName="Get Total Count of Offers", meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject"),
		Category="Thirdweb|Engine|Marketplace|Offers|Read")
	static UAsyncTaskOffersGetTotalCount* MarketplaceGetTotalCount(
		UObject* WorldContextObject,
		const UThirdwebMarketplace* Marketplace
	);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGetTotalCountDelegate, const int64&, Total, const FString&, Error);

	UPROPERTY(BlueprintAssignable)
	FGetTotalCountDelegate Success;

	UPROPERTY(BlueprintAssignable)
	FGetTotalCountDelegate Failed;

private:
	virtual void HandleResponse(const FString& Total);
	void HandleFailed(const FString& Error);

public:
	virtual void Activate() override;
};