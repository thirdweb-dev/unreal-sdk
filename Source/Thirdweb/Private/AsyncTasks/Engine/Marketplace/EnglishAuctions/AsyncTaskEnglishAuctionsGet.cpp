// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Engine/Marketplace/EnglishAuctions/AsyncTaskEnglishAuctionsGet.h"

#include "Components/SlateWrapperTypes.h"
#include "Engine/ThirdwebEngine.h"
#include "Engine/Marketplace/ThirdwebMarketplace.h"
#include "Engine/Marketplace/DirectListings/ThirdwebMarketplaceDirectListing.h"
#include "Engine/Marketplace/EnglishAuctions/ThirdwebMarketplaceEnglishAuction.h"

UAsyncTaskEnglishAuctionsGet* UAsyncTaskEnglishAuctionsGet::Get(
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

UAsyncTaskEnglishAuctionsGet* UAsyncTaskEnglishAuctionsGet::MarketplaceGet(
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

void UAsyncTaskEnglishAuctionsGet::HandleResponse(const FThirdwebMarketplaceEnglishAuction& Listing)
{
	Success.Broadcast(Listing, TEXT(""));
	SetReadyToDestroy();
}

void UAsyncTaskEnglishAuctionsGet::HandleFailed(const FString& Error)
{
	Failed.Broadcast(FThirdwebMarketplaceEnglishAuction(), Error);
	SetReadyToDestroy();
}

void UAsyncTaskEnglishAuctionsGet::Activate()
{
	ThirdwebEngine::Marketplace::EnglishAuctions::Get(
		this,
		ListingId,
		Chain,
		MarketplaceContract,
		BIND_UOBJECT_DELEGATE(ThirdwebEngine::Marketplace::EnglishAuctions::FGetAuctionDelegate, HandleResponse),
		BIND_UOBJECT_DELEGATE(FStringDelegate, HandleFailed)
	);
}
