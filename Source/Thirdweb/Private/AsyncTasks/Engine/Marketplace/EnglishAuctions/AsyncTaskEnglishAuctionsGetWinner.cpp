// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Engine/Marketplace/EnglishAuctions/AsyncTaskEnglishAuctionsGetWinner.h"

#include "ThirdwebUtils.h"
#include "Components/SlateWrapperTypes.h"
#include "Engine/ThirdwebEngine.h"
#include "Engine/Marketplace/ThirdwebMarketplace.h"

UAsyncTaskEnglishAuctionsGetWinner* UAsyncTaskEnglishAuctionsGetWinner::GetWinner(
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

UAsyncTaskEnglishAuctionsGetWinner* UAsyncTaskEnglishAuctionsGetWinner::MarketplaceGetWinner(
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

void UAsyncTaskEnglishAuctionsGetWinner::HandleResponse(const FString& Winner)
{
	Success.Broadcast(Winner, TEXT(""));
	SetReadyToDestroy();
}

void UAsyncTaskEnglishAuctionsGetWinner::HandleFailed(const FString& Error)
{
	Failed.Broadcast(TEXT(""), Error);
	SetReadyToDestroy();
}

void UAsyncTaskEnglishAuctionsGetWinner::Activate()
{
	ThirdwebEngine::Marketplace::EnglishAuctions::GetWinner(
		this,
		ListingId,
		Chain,
		MarketplaceContract,
		BIND_UOBJECT_DELEGATE(FStringDelegate, HandleResponse),
		BIND_UOBJECT_DELEGATE(FStringDelegate, HandleFailed)
	);
}
