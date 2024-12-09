// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Engine/Marketplace/EnglishAuctions/AsyncTaskEnglishAuctionsGetAll.h"

#include "Components/SlateWrapperTypes.h"
#include "Engine/ThirdwebEngine.h"
#include "Engine/Marketplace/ThirdwebMarketplace.h"

UAsyncTaskEnglishAuctionsGetAll* UAsyncTaskEnglishAuctionsGetAll::GetAll(
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

UAsyncTaskEnglishAuctionsGetAll* UAsyncTaskEnglishAuctionsGetAll::MarketplaceGetAll(
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

void UAsyncTaskEnglishAuctionsGetAll::HandleResponse(const TArray<FThirdwebMarketplaceEnglishAuction>& Listings)
{
	Success.Broadcast(Listings, TEXT(""));
	SetReadyToDestroy();
}

void UAsyncTaskEnglishAuctionsGetAll::HandleFailed(const FString& Error)
{
	Failed.Broadcast({}, Error);
	SetReadyToDestroy();
}

void UAsyncTaskEnglishAuctionsGetAll::Activate()
{
	ThirdwebEngine::Marketplace::EnglishAuctions::GetAll(
		this,
		Count,
		Seller,
		Start,
		TokenContract,
		TokenId,
		Chain,
		MarketplaceContract,
		bOnlyValid,
		BIND_UOBJECT_DELEGATE(ThirdwebEngine::Marketplace::EnglishAuctions::FGetAllDelegate, HandleResponse),
		BIND_UOBJECT_DELEGATE(FStringDelegate, HandleFailed)
	);
}
