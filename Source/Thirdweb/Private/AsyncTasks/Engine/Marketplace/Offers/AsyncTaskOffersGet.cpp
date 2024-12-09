// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Engine/Marketplace/Offers/AsyncTaskOffersGet.h"

#include "Components/SlateWrapperTypes.h"
#include "Engine/ThirdwebEngine.h"
#include "Engine/Marketplace/ThirdwebMarketplace.h"
#include "Engine/Marketplace/DirectListings/ThirdwebMarketplaceDirectListing.h"
#include "Engine/Marketplace/Offers/ThirdwebMarketplaceOffer.h"

UAsyncTaskOffersGet* UAsyncTaskOffersGet::Get(
	UObject* WorldContextObject,
	const int64 ChainId,
	const FString& Contract,
	const FString& OfferId
)
{
	NEW_TASK
	Task->OfferId = OfferId;
	Task->Chain = ChainId;
	Task->MarketplaceContract = Contract;
	RR_TASK
}

UAsyncTaskOffersGet* UAsyncTaskOffersGet::MarketplaceGet(
	UObject* WorldContextObject,
	const UThirdwebMarketplace* Marketplace,
	const FString& OfferId
)
{
	NEW_MARKETPLACE_TASK
	Task->OfferId = OfferId;
	Task->Chain = Marketplace->GetChainId();
	Task->MarketplaceContract = Marketplace->GetContractAddress();
	RR_TASK
}

void UAsyncTaskOffersGet::HandleResponse(const FThirdwebMarketplaceOffer& Offer)
{
	Success.Broadcast(Offer, TEXT(""));
	SetReadyToDestroy();
}

void UAsyncTaskOffersGet::HandleFailed(const FString& Error)
{
	Failed.Broadcast(FThirdwebMarketplaceOffer(), Error);
	SetReadyToDestroy();
}

void UAsyncTaskOffersGet::Activate()
{
	ThirdwebEngine::Marketplace::Offers::Get(
		this,
		OfferId,
		Chain,
		MarketplaceContract,
		BIND_UOBJECT_DELEGATE(ThirdwebEngine::Marketplace::Offers::FGetOfferDelegate, HandleResponse),
		BIND_UOBJECT_DELEGATE(FStringDelegate, HandleFailed)
	);
}
