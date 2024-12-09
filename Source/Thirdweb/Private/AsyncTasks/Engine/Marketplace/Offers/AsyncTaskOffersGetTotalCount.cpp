// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Engine/Marketplace/Offers/AsyncTaskOffersGetTotalCount.h"

#include "ThirdwebUtils.h"
#include "Components/SlateWrapperTypes.h"
#include "Engine/ThirdwebEngine.h"
#include "Engine/Marketplace/ThirdwebMarketplace.h"

UAsyncTaskOffersGetTotalCount* UAsyncTaskOffersGetTotalCount::GetTotalCount(
	UObject* WorldContextObject,
	const int64 ChainId,
	const FString& Contract
)
{
	NEW_TASK
	Task->Chain = ChainId;
	Task->MarketplaceContract = Contract;
	RR_TASK
}

UAsyncTaskOffersGetTotalCount* UAsyncTaskOffersGetTotalCount::MarketplaceGetTotalCount(
	UObject* WorldContextObject,
	const UThirdwebMarketplace* Marketplace
)
{
	NEW_MARKETPLACE_TASK
	Task->Chain = Marketplace->GetChainId();
	Task->MarketplaceContract = Marketplace->GetContractAddress();
	RR_TASK
}

void UAsyncTaskOffersGetTotalCount::HandleResponse(const FString& Total)
{
	Success.Broadcast(ThirdwebUtils::Internal::ParseInt64(Total), TEXT(""));
	SetReadyToDestroy();
}

void UAsyncTaskOffersGetTotalCount::HandleFailed(const FString& Error)
{
	Failed.Broadcast(0, Error);
	SetReadyToDestroy();
}

void UAsyncTaskOffersGetTotalCount::Activate()
{
	ThirdwebEngine::Marketplace::Offers::GetTotalCount(
		this,
		Chain,
		MarketplaceContract,
		BIND_UOBJECT_DELEGATE(FStringDelegate, HandleResponse),
		BIND_UOBJECT_DELEGATE(FStringDelegate, HandleFailed)
	);
}
