// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Engine/Marketplace/DirectListings/AsyncTaskThirdwebEngineMarketplaceDirectListingsRevokeReservedBuyerApproval.h"

#include "Components/SlateWrapperTypes.h"
#include "Engine/ThirdwebEngine.h"
#include "Engine/Marketplace/ThirdwebMarketplace.h"

UAsyncTaskThirdwebEngineMarketplaceDirectListingsRevokeReservedBuyerApproval* UAsyncTaskThirdwebEngineMarketplaceDirectListingsRevokeReservedBuyerApproval::RevokeReservedBuyerApproval(
	UObject* WorldContextObject,
	const int64 ChainId,
	const FString& Contract,
	const FString& BackendWallet,
	const FSmartWalletHandle& Account,
	const FString& ListingId,
	const FString& Buyer,
	const FString& IdempotencyKey,
	const bool bSimulateTx
)
{
	NEW_TASK
	Task->Chain = ChainId;
	Task->MarketplaceContract = Contract;
	Task->BackendWalletAddress = BackendWallet;
	Task->SmartWallet = Account;
	Task->ListingId = ListingId;
	Task->Buyer = Buyer;
	Task->Idempotency = IdempotencyKey;
	Task->bSimulateTransaction = bSimulateTx;
	RR_TASK
}

UAsyncTaskThirdwebEngineMarketplaceDirectListingsRevokeReservedBuyerApproval* UAsyncTaskThirdwebEngineMarketplaceDirectListingsRevokeReservedBuyerApproval::MarketplaceRevokeReservedBuyerApproval(
	UObject* WorldContextObject,
	const UThirdwebMarketplace* Marketplace,
	const FString& BackendWallet,
	const FSmartWalletHandle& Account,
	const FString& ListingId,
	const FString& Buyer,
	const FString& IdempotencyKey,
	const bool bSimulateTx
)
{
	NEW_MARKETPLACE_TASK
	Task->Chain = Marketplace->GetChainId();
	Task->MarketplaceContract = Marketplace->GetContractAddress();
	Task->BackendWalletAddress = BackendWallet;
	Task->SmartWallet = Account;
	Task->ListingId = ListingId;
	Task->Buyer = Buyer;
	Task->Idempotency = IdempotencyKey;
	Task->bSimulateTransaction = bSimulateTx;
	RR_TASK
}

void UAsyncTaskThirdwebEngineMarketplaceDirectListingsRevokeReservedBuyerApproval::Activate()
{
	ThirdwebEngine::Marketplace::DirectListings::RevokeReservedBuyerApproval(
		this,
		Chain,
		MarketplaceContract,
		BackendWalletAddress,
		{SmartWallet},
		ListingId,
		Buyer,
		Idempotency,
		bSimulateTransaction,
		BIND_UOBJECT_DELEGATE(FStringDelegate, HandleResponse),
		BIND_UOBJECT_DELEGATE(FStringDelegate, HandleFailed)
	);
}
