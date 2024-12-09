// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Engine/Marketplace/DirectListings/AsyncTaskDirectListingsIsBuyerApproved.h"

#include "Components/SlateWrapperTypes.h"
#include "Engine/ThirdwebEngine.h"
#include "Engine/Marketplace/ThirdwebMarketplace.h"

UAsyncTaskDirectListingsIsBuyerApproved* UAsyncTaskDirectListingsIsBuyerApproved::IsBuyerApproved(
	UObject* WorldContextObject,
	const int64 ChainId,
	const FString& Contract,
	const FString& ListingId,
	const FString& Buyer
)
{
	NEW_TASK
	Task->Chain = ChainId;
	Task->MarketplaceContract = Contract;
	Task->ListingId = ListingId;
	Task->WalletAddress = Buyer;
	RR_TASK
}

UAsyncTaskDirectListingsIsBuyerApproved* UAsyncTaskDirectListingsIsBuyerApproved::MarketplaceIsBuyerApproved(
	UObject* WorldContextObject,
	const UThirdwebMarketplace* Marketplace,
	const FString& ListingId,
	const FString& Buyer
)
{
	NEW_MARKETPLACE_TASK
	Task->Chain = Marketplace->GetChainId();
	Task->MarketplaceContract = Marketplace->GetContractAddress();
	Task->ListingId = ListingId;
	Task->WalletAddress = Buyer;
	RR_TASK
}

void UAsyncTaskDirectListingsIsBuyerApproved::HandleResponse(const bool bApproved)
{
	Success.Broadcast(bApproved, TEXT(""));
	SetReadyToDestroy();
}

void UAsyncTaskDirectListingsIsBuyerApproved::HandleFailed(const FString& Error)
{
	Failed.Broadcast(false, Error);
	SetReadyToDestroy();
}

void UAsyncTaskDirectListingsIsBuyerApproved::Activate()
{
	ThirdwebEngine::Marketplace::DirectListings::IsBuyerApproved(
		this,
		ListingId,
		WalletAddress,
		Chain,
		MarketplaceContract,
		BIND_UOBJECT_DELEGATE(FBoolDelegate, HandleResponse),
		BIND_UOBJECT_DELEGATE(FStringDelegate, HandleFailed)
	);
}
