// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Engine/Marketplace/DirectListings/AsyncTaskThirdwebEngineMarketplaceDirectListingsCreate.h"

#include "Components/SlateWrapperTypes.h"
#include "Engine/ThirdwebEngine.h"
#include "Engine/Marketplace/ThirdwebMarketplace.h"

UAsyncTaskThirdwebEngineMarketplaceDirectListingsCreate* UAsyncTaskThirdwebEngineMarketplaceDirectListingsCreate::Create(
	UObject* WorldContextObject,
	const int64 ChainId,
	const FString& Contract,
	const FString& BackendWallet,
	const FSmartWalletHandle& Account,
	const FThirdwebMarketplaceCreateListingRequest& Listing,
	const FString& IdempotencyKey,
	const bool bSimulateTx
)
{
	NEW_TASK
	Task->Chain = ChainId;
	Task->MarketplaceContract = Contract;
	Task->BackendWalletAddress = BackendWallet;
	Task->SmartWallet = Account;
	Task->Listing = Listing;
	Task->Idempotency = IdempotencyKey;
	Task->bSimulateTransaction = bSimulateTx;
	RR_TASK
}

UAsyncTaskThirdwebEngineMarketplaceDirectListingsCreate* UAsyncTaskThirdwebEngineMarketplaceDirectListingsCreate::MarketplaceCreate(
	UObject* WorldContextObject,
	const UThirdwebMarketplace* Marketplace,
	const FString& BackendWallet,
	const FSmartWalletHandle& Account,
	const FThirdwebMarketplaceCreateListingRequest& Listing,
	const FString& IdempotencyKey,
	const bool bSimulateTx)
{
	NEW_MARKETPLACE_TASK
	Task->Chain = Marketplace->GetChainId();
	Task->MarketplaceContract = Marketplace->GetContractAddress();
	Task->BackendWalletAddress = BackendWallet;
	Task->SmartWallet = Account;
	Task->Listing = Listing;
	Task->Idempotency = IdempotencyKey;
	Task->bSimulateTransaction = bSimulateTx;
	RR_TASK
}

void UAsyncTaskThirdwebEngineMarketplaceDirectListingsCreate::Activate()
{
	ThirdwebEngine::Marketplace::DirectListings::Create(
		this,
		Chain,
		MarketplaceContract,
		BackendWalletAddress,
		{SmartWallet},
		Listing,
		Idempotency,
		bSimulateTransaction,
		BIND_UOBJECT_DELEGATE(FStringDelegate, HandleResponse),
		BIND_UOBJECT_DELEGATE(FStringDelegate, HandleFailed)
	);
}