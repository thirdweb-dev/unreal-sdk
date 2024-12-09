// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Engine/Marketplace/EnglishAuctions/AsyncTaskEnglishAuctionsGetBidBufferBps.h"

#include "ThirdwebUtils.h"
#include "Components/SlateWrapperTypes.h"
#include "Engine/ThirdwebEngine.h"
#include "Engine/Marketplace/ThirdwebMarketplace.h"

UAsyncTaskEnglishAuctionsGetBidBufferBps* UAsyncTaskEnglishAuctionsGetBidBufferBps::GetBidBufferBps(
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

UAsyncTaskEnglishAuctionsGetBidBufferBps* UAsyncTaskEnglishAuctionsGetBidBufferBps::MarketplaceGetBidBufferBps(
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

void UAsyncTaskEnglishAuctionsGetBidBufferBps::HandleResponse(const int32 Bps)
{
	Success.Broadcast(Bps, TEXT(""));
	SetReadyToDestroy();
}

void UAsyncTaskEnglishAuctionsGetBidBufferBps::HandleFailed(const FString& Error)
{
	Failed.Broadcast(0, Error);
	SetReadyToDestroy();
}

void UAsyncTaskEnglishAuctionsGetBidBufferBps::Activate()
{
	ThirdwebEngine::Marketplace::EnglishAuctions::GetBidBufferBps(
		this,
		ListingId,
		Chain,
		MarketplaceContract,
		BIND_UOBJECT_DELEGATE(FInt32Delegate, HandleResponse),
		BIND_UOBJECT_DELEGATE(FStringDelegate, HandleFailed)
	);
}
