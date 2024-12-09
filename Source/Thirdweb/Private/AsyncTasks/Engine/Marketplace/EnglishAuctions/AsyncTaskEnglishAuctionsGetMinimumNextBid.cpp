// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Engine/Marketplace/EnglishAuctions/AsyncTaskEnglishAuctionsGetMinimumNextBid.h"

#include "ThirdwebUtils.h"
#include "Components/SlateWrapperTypes.h"
#include "Engine/ThirdwebEngine.h"
#include "Engine/Marketplace/ThirdwebMarketplace.h"

UAsyncTaskEnglishAuctionsGetMinimumNextBid* UAsyncTaskEnglishAuctionsGetMinimumNextBid::GetMinimumNextBid(
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

UAsyncTaskEnglishAuctionsGetMinimumNextBid* UAsyncTaskEnglishAuctionsGetMinimumNextBid::MarketplaceGetMinimumNextBid(
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

void UAsyncTaskEnglishAuctionsGetMinimumNextBid::HandleResponse(const FString& MinimumNextBid)
{
	Success.Broadcast(MinimumNextBid, TEXT(""));
	SetReadyToDestroy();
}

void UAsyncTaskEnglishAuctionsGetMinimumNextBid::HandleFailed(const FString& Error)
{
	Failed.Broadcast(TEXT("0"), Error);
	SetReadyToDestroy();
}

void UAsyncTaskEnglishAuctionsGetMinimumNextBid::Activate()
{
	ThirdwebEngine::Marketplace::EnglishAuctions::GetMinimumNextBid(
		this,
		ListingId,
		Chain,
		MarketplaceContract,
		BIND_UOBJECT_DELEGATE(FStringDelegate, HandleResponse),
		BIND_UOBJECT_DELEGATE(FStringDelegate, HandleFailed)
	);
}
