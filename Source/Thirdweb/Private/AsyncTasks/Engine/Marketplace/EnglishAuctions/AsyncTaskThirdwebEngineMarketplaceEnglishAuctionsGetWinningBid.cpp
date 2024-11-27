// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Engine/Marketplace/EnglishAuctions/AsyncTaskThirdwebEngineMarketplaceEnglishAuctionsGetWinningBid.h"

#include "ThirdwebUtils.h"
#include "Components/SlateWrapperTypes.h"
#include "Engine/ThirdwebEngine.h"
#include "Engine/Marketplace/ThirdwebMarketplace.h"
#include "Engine/Marketplace/EnglishAuctions/ThirdwebMarketplaceBId.h"

UAsyncTaskThirdwebEngineMarketplaceEnglishAuctionsGetWinningBid* UAsyncTaskThirdwebEngineMarketplaceEnglishAuctionsGetWinningBid::GetWinningBid(
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

UAsyncTaskThirdwebEngineMarketplaceEnglishAuctionsGetWinningBid* UAsyncTaskThirdwebEngineMarketplaceEnglishAuctionsGetWinningBid::MarketplaceGetWinningBid(
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

void UAsyncTaskThirdwebEngineMarketplaceEnglishAuctionsGetWinningBid::HandleResponse(const FThirdwebMarketplaceBid& WinningBid)
{
	Success.Broadcast(WinningBid, TEXT(""));
	SetReadyToDestroy();
}

void UAsyncTaskThirdwebEngineMarketplaceEnglishAuctionsGetWinningBid::HandleFailed(const FString& Error)
{
	Failed.Broadcast(FThirdwebMarketplaceBid(), Error);
	SetReadyToDestroy();
}

void UAsyncTaskThirdwebEngineMarketplaceEnglishAuctionsGetWinningBid::Activate()
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
