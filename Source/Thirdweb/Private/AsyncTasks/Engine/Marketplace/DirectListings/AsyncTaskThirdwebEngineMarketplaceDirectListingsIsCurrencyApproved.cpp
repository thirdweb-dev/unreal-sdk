// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Engine/Marketplace/DirectListings/AsyncTaskThirdwebEngineMarketplaceDirectListingsIsCurrencyApproved.h"

#include "Components/SlateWrapperTypes.h"
#include "Engine/ThirdwebEngine.h"
#include "Engine/Marketplace/ThirdwebMarketplace.h"

UAsyncTaskThirdwebEngineMarketplaceDirectListingsIsCurrencyApproved* UAsyncTaskThirdwebEngineMarketplaceDirectListingsIsCurrencyApproved::IsCurrencyApproved(
	UObject* WorldContextObject,
	const int64 ChainId,
	const FString& Contract,
	const FString& ListingId,
	const FString& Currency
)
{
	NEW_TASK
	Task->Chain = ChainId;
	Task->MarketplaceContract = Contract;
	Task->ListingId = ListingId;
	Task->CurrencyContractAddress = Currency;
	RR_TASK
}

UAsyncTaskThirdwebEngineMarketplaceDirectListingsIsCurrencyApproved* UAsyncTaskThirdwebEngineMarketplaceDirectListingsIsCurrencyApproved::MarketplaceIsCurrencyApproved(
	UObject* WorldContextObject,
	const UThirdwebMarketplace* Marketplace,
	const FString& ListingId,
	const FString& Currency
)
{
	NEW_MARKETPLACE_TASK
	Task->Chain = Marketplace->GetChainId();
	Task->MarketplaceContract = Marketplace->GetContractAddress();
	Task->ListingId = ListingId;
	Task->CurrencyContractAddress = Currency;
	RR_TASK
}

void UAsyncTaskThirdwebEngineMarketplaceDirectListingsIsCurrencyApproved::HandleResponse(const bool bApproved)
{
	Success.Broadcast(bApproved, TEXT(""));
	SetReadyToDestroy();
}

void UAsyncTaskThirdwebEngineMarketplaceDirectListingsIsCurrencyApproved::HandleFailed(const FString& Error)
{
	Failed.Broadcast(false, Error);
	SetReadyToDestroy();
}

void UAsyncTaskThirdwebEngineMarketplaceDirectListingsIsCurrencyApproved::Activate()
{
	ThirdwebEngine::Marketplace::DirectListings::IsCurrencyApproved(
		this,
		ListingId,
		CurrencyContractAddress,
		Chain,
		MarketplaceContract,
		BIND_UOBJECT_DELEGATE(FBoolDelegate, HandleResponse),
		BIND_UOBJECT_DELEGATE(FStringDelegate, HandleFailed)
	);
}
