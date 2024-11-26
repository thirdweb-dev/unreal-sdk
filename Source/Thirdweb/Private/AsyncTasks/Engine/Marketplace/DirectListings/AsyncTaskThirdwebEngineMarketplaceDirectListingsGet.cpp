// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Engine/Marketplace/DirectListings/AsyncTaskThirdwebEngineMarketplaceDirectListingsGet.h"

#include "Components/SlateWrapperTypes.h"
#include "Engine/ThirdwebEngine.h"
#include "Engine/Marketplace/ThirdwebMarketplace.h"
#include "Engine/Marketplace/DirectListings/ThirdwebMarketplaceListing.h"

UAsyncTaskThirdwebEngineMarketplaceDirectListingsGet* UAsyncTaskThirdwebEngineMarketplaceDirectListingsGet::Get(
	UObject* WorldContextObject,
	const int64 ChainId,
	const FString& Contract,
	const FString& ListingId
)
{
	NEW_TASK
	Task->Chain = ChainId;
	Task->MarketplaceContract = Contract;
	Task->ListingId = ListingId;
	RR_TASK
}

UAsyncTaskThirdwebEngineMarketplaceDirectListingsGet* UAsyncTaskThirdwebEngineMarketplaceDirectListingsGet::MarketplaceGet(
	UObject* WorldContextObject,
	const UThirdwebMarketplace* Marketplace,
	const FString& ListingId
)
{
	NEW_MARKETPLACE_TASK
	Task->Chain = Marketplace->GetChainId();
	Task->MarketplaceContract = Marketplace->GetContractAddress();
	Task->ListingId = ListingId;
	RR_TASK
}

void UAsyncTaskThirdwebEngineMarketplaceDirectListingsGet::HandleResponse(const FThirdwebMarketplaceListing& Listing)
{
	Success.Broadcast(Listing, TEXT(""));
	SetReadyToDestroy();
}

void UAsyncTaskThirdwebEngineMarketplaceDirectListingsGet::HandleFailed(const FString& Error)
{
	Failed.Broadcast(FThirdwebMarketplaceListing(), Error);
	SetReadyToDestroy();
}

void UAsyncTaskThirdwebEngineMarketplaceDirectListingsGet::Activate()
{
	ThirdwebEngine::Marketplace::DirectListings::Get(
		this,
		ListingId,
		Chain,
		MarketplaceContract,
		BIND_UOBJECT_DELEGATE(ThirdwebEngine::Marketplace::DirectListings::FGetListingDelegate, HandleResponse),
		BIND_UOBJECT_DELEGATE(FStringDelegate, HandleFailed)
	);
}
