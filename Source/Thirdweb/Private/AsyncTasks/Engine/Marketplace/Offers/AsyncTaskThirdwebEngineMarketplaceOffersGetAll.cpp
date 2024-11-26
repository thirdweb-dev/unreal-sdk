// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Engine/Marketplace/Offers/AsyncTaskThirdwebEngineMarketplaceOffersGetAll.h"

#include "Components/SlateWrapperTypes.h"
#include "Engine/ThirdwebEngine.h"
#include "Engine/Marketplace/ThirdwebMarketplace.h"

UAsyncTaskThirdwebEngineMarketplaceOffersGetAll* UAsyncTaskThirdwebEngineMarketplaceOffersGetAll::GetAll(
	UObject* WorldContextObject,
	const int64 ChainId,
	const FString& Contract,
	const bool bOnlyValid,
	const int32 Count,
	const int32 Start,
	const FString& Offeror,
	const FString& TokenAddress,
	const FString& TokenId
)
{
	NEW_TASK
	Task->Count = Count;
	Task->bOnlyValid = bOnlyValid;
	Task->Start = Start;
	Task->Offeror = Offeror;
	Task->TokenContract = TokenAddress;
	Task->TokenId = TokenId;
	Task->Chain = ChainId;
	Task->MarketplaceContract = Contract;
	RR_TASK
}

UAsyncTaskThirdwebEngineMarketplaceOffersGetAll* UAsyncTaskThirdwebEngineMarketplaceOffersGetAll::MarketplaceGetAll(
	UObject* WorldContextObject,
	const UThirdwebMarketplace* Marketplace,
	const bool bOnlyValid,
	const int32 Count,
	const int32 Start,
	const FString& Offeror,
	const FString& TokenAddress,
	const FString& TokenId
)
{
	NEW_MARKETPLACE_TASK
	Task->Count = Count;
	Task->bOnlyValid = bOnlyValid;
	Task->Start = Start;
	Task->Offeror = Offeror;
	Task->TokenContract = TokenAddress;
	Task->TokenId = TokenId;
	Task->Chain = Marketplace->GetChainId();
	Task->MarketplaceContract = Marketplace->GetContractAddress();
	RR_TASK
}

void UAsyncTaskThirdwebEngineMarketplaceOffersGetAll::HandleResponse(const TArray<FThirdwebMarketplaceOffer>& Offers)
{
	Success.Broadcast(Offers, TEXT(""));
	SetReadyToDestroy();
}

void UAsyncTaskThirdwebEngineMarketplaceOffersGetAll::HandleFailed(const FString& Error)
{
	Failed.Broadcast({}, Error);
	SetReadyToDestroy();
}

void UAsyncTaskThirdwebEngineMarketplaceOffersGetAll::Activate()
{
	ThirdwebEngine::Marketplace::Offers::GetAll(
		this,
		Count,
		Offeror,
		Start,
		TokenContract,
		TokenId,
		Chain,
		MarketplaceContract,
		bOnlyValid,
		BIND_UOBJECT_DELEGATE(ThirdwebEngine::Marketplace::Offers::FGetAllDelegate, HandleResponse),
		BIND_UOBJECT_DELEGATE(FStringDelegate, HandleFailed)
	);
}
