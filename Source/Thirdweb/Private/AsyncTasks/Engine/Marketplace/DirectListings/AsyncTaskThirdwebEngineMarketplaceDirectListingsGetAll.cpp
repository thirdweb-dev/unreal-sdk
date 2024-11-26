// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Engine/Marketplace/DirectListings/AsyncTaskThirdwebEngineMarketplaceDirectListingsGetAll.h"

#include "Components/SlateWrapperTypes.h"
#include "Engine/ThirdwebEngine.h"
#include "Engine/Marketplace/ThirdwebMarketplace.h"

UAsyncTaskThirdwebEngineMarketplaceDirectListingsGetAll* UAsyncTaskThirdwebEngineMarketplaceDirectListingsGetAll::GetAll(
	UObject* WorldContextObject,
	const int64 ChainId,
	const FString& Contract,
	const bool bOnlyValid,
	const int32 Count,
	const int32 Start,
	const FString& Seller,
	const FString& TokenAddress,
	const FString& TokenId
)
{
	NEW_TASK
	Task->Count = Count;
	Task->bOnlyValid = bOnlyValid;
	Task->Start = Start;
	Task->Seller = Seller;
	Task->TokenContract = TokenAddress;
	Task->TokenId = TokenId;
	Task->Chain = ChainId;
	Task->MarketplaceContract = Contract;
	RR_TASK
}

UAsyncTaskThirdwebEngineMarketplaceDirectListingsGetAll* UAsyncTaskThirdwebEngineMarketplaceDirectListingsGetAll::MarketplaceGetAll(
	UObject* WorldContextObject,
	const UThirdwebMarketplace* Marketplace,
	const bool bOnlyValid,
	const int32 Count,
	const int32 Start,
	const FString& Seller,
	const FString& TokenAddress,
	const FString& TokenId
)
{
	NEW_MARKETPLACE_TASK
	Task->Count = Count;
	Task->bOnlyValid = bOnlyValid;
	Task->Start = Start;
	Task->Seller = Seller;
	Task->TokenContract = TokenAddress;
	Task->TokenId = TokenId;
	Task->Chain = Marketplace->GetChainId();
	Task->MarketplaceContract = Marketplace->GetContractAddress();
	RR_TASK
}

void UAsyncTaskThirdwebEngineMarketplaceDirectListingsGetAll::HandleResponse(const TArray<FThirdwebMarketplaceListing>& Listings)
{
	Success.Broadcast(Listings, TEXT(""));
	SetReadyToDestroy();
}

void UAsyncTaskThirdwebEngineMarketplaceDirectListingsGetAll::HandleFailed(const FString& Error)
{
	Failed.Broadcast({}, Error);
	SetReadyToDestroy();
}

void UAsyncTaskThirdwebEngineMarketplaceDirectListingsGetAll::Activate()
{
	ThirdwebEngine::Marketplace::DirectListings::GetAll(
		this,
		Count,
		Seller,
		Start,
		TokenContract,
		TokenId,
		Chain,
		MarketplaceContract,
		bOnlyValid,
		BIND_UOBJECT_DELEGATE(ThirdwebEngine::Marketplace::DirectListings::FGetAllDelegate, HandleResponse),
		BIND_UOBJECT_DELEGATE(FStringDelegate, HandleFailed)
	);
}
