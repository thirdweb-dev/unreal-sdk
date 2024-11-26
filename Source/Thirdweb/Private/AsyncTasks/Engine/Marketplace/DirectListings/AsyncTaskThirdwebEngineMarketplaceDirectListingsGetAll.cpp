// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Engine/Marketplace/DirectListings/AsyncTaskThirdwebEngineMarketplaceDirectListingsGetAll.h"

#include "Components/SlateWrapperTypes.h"
#include "Engine/ThirdwebEngine.h"

UAsyncTaskThirdwebEngineMarketplaceDirectListingsGetAll* UAsyncTaskThirdwebEngineMarketplaceDirectListingsGetAll::GetAllDirectListings(
	UObject* WorldContextObject,
	const int32 Count,
	const FString& Seller,
	const int32 Start,
	const FString& TokenContract,
	const FString& TokenId,
	const int64 Chain,
	const FString& ContractAddress
)
{
	NEW_TASK
	Task->Count = Count;
	Task->Start = Start;
	Task->Seller = Seller;
	Task->TokenContract = TokenContract;
	Task->TokenId = TokenId;
	Task->Chain = Chain;
	Task->ContractAddress = ContractAddress;
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
		ContractAddress,
		BIND_UOBJECT_DELEGATE(ThirdwebEngine::Marketplace::DirectListings::FGetAllDelegate, HandleResponse),
		BIND_UOBJECT_DELEGATE(FStringDelegate, HandleFailed)
	);
}
