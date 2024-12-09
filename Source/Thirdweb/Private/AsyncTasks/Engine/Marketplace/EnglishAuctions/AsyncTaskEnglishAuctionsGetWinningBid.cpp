// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Engine/Marketplace/EnglishAuctions/AsyncTaskEnglishAuctionsGetWinningBid.h"

#include "ThirdwebUtils.h"
#include "Components/SlateWrapperTypes.h"
#include "Engine/ThirdwebEngine.h"
#include "Engine/Marketplace/ThirdwebMarketplace.h"
#include "Engine/Marketplace/EnglishAuctions/ThirdwebMarketplaceBid.h"

UAsyncTaskEnglishAuctionsGetWinningBid* UAsyncTaskEnglishAuctionsGetWinningBid::GetWinningBid(
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

UAsyncTaskEnglishAuctionsGetWinningBid* UAsyncTaskEnglishAuctionsGetWinningBid::MarketplaceGetWinningBid(
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

void UAsyncTaskEnglishAuctionsGetWinningBid::HandleResponse(const FThirdwebMarketplaceBid& WinningBid)
{
	Success.Broadcast(WinningBid, TEXT(""));
	SetReadyToDestroy();
}

void UAsyncTaskEnglishAuctionsGetWinningBid::HandleFailed(const FString& Error)
{
	Failed.Broadcast(FThirdwebMarketplaceBid(), Error);
	SetReadyToDestroy();
}

void UAsyncTaskEnglishAuctionsGetWinningBid::Activate()
{
	ThirdwebEngine::Marketplace::EnglishAuctions::GetWinningBid(
		this,
		ListingId,
		Chain,
		MarketplaceContract,
		BIND_UOBJECT_DELEGATE(ThirdwebEngine::Marketplace::EnglishAuctions::FGetBidDelegate, HandleResponse),
		BIND_UOBJECT_DELEGATE(FStringDelegate, HandleFailed)
	);
}
