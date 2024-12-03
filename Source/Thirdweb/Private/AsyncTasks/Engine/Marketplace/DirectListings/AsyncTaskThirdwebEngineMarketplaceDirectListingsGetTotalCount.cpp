// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Engine/Marketplace/DirectListings/AsyncTaskThirdwebEngineMarketplaceDirectListingsGetTotalCount.h"

#include "ThirdwebUtils.h"
#include "Components/SlateWrapperTypes.h"
#include "Engine/ThirdwebEngine.h"
#include "Engine/Marketplace/ThirdwebMarketplace.h"

UAsyncTaskThirdwebEngineMarketplaceDirectListingsGetTotalCount* UAsyncTaskThirdwebEngineMarketplaceDirectListingsGetTotalCount::GetTotalCount(
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

UAsyncTaskThirdwebEngineMarketplaceDirectListingsGetTotalCount* UAsyncTaskThirdwebEngineMarketplaceDirectListingsGetTotalCount::MarketplaceGetTotalCount(
	UObject* WorldContextObject,
	const UThirdwebMarketplace* Marketplace
)
{
	NEW_MARKETPLACE_TASK
	Task->Chain = Marketplace->GetChainId();
	Task->MarketplaceContract = Marketplace->GetContractAddress();
	RR_TASK
}

void UAsyncTaskThirdwebEngineMarketplaceDirectListingsGetTotalCount::HandleResponse(const FString& Total)
{
	Success.Broadcast(ThirdwebUtils::Internal::ParseInt64(Total), TEXT(""));
	SetReadyToDestroy();
}

void UAsyncTaskThirdwebEngineMarketplaceDirectListingsGetTotalCount::HandleFailed(const FString& Error)
{
	Failed.Broadcast(0, Error);
	SetReadyToDestroy();
}

void UAsyncTaskThirdwebEngineMarketplaceDirectListingsGetTotalCount::Activate()
{
	ThirdwebEngine::Marketplace::DirectListings::GetTotalCount(
		this,
		Chain,
		MarketplaceContract,
		BIND_UOBJECT_DELEGATE(FStringDelegate, HandleResponse),
		BIND_UOBJECT_DELEGATE(FStringDelegate, HandleFailed)
	);
}
