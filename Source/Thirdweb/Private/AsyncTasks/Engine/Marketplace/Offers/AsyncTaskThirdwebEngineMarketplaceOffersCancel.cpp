// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Engine/Marketplace/Offers/AsyncTaskThirdwebEngineMarketplaceOffersCancel.h"

#include "Components/SlateWrapperTypes.h"
#include "Engine/ThirdwebEngine.h"
#include "Engine/Marketplace/ThirdwebMarketplace.h"

UAsyncTaskThirdwebEngineMarketplaceOffersCancel* UAsyncTaskThirdwebEngineMarketplaceOffersCancel::Cancel(
	UObject* WorldContextObject,
	const int64 ChainId,
	const FString& Contract,
	const FString& BackendWallet,
	const FSmartWalletHandle& Account,
	const FString& OfferId,
	const FString& IdempotencyKey,
	const bool bSimulateTx
)
{
	NEW_TASK
	Task->Chain = ChainId;
	Task->MarketplaceContract = Contract;
	Task->BackendWalletAddress = BackendWallet;
	Task->SmartWallet = Account;
	Task->OfferId = OfferId;
	Task->Idempotency = IdempotencyKey;
	Task->bSimulateTransaction = bSimulateTx;
	RR_TASK
}

UAsyncTaskThirdwebEngineMarketplaceOffersCancel* UAsyncTaskThirdwebEngineMarketplaceOffersCancel::MarketplaceCancel(
	UObject* WorldContextObject,
	const UThirdwebMarketplace* Marketplace,
	const FString& BackendWallet,
	const FSmartWalletHandle& Account,
	const FString& OfferId,
	const FString& IdempotencyKey,
	const bool bSimulateTx
)
{
	NEW_MARKETPLACE_TASK
	Task->Chain = Marketplace->GetChainId();
	Task->MarketplaceContract = Marketplace->GetContractAddress();
	Task->BackendWalletAddress = BackendWallet;
	Task->SmartWallet = Account;
	Task->OfferId = OfferId;
	Task->Idempotency = IdempotencyKey;
	Task->bSimulateTransaction = bSimulateTx;
	RR_TASK
}

void UAsyncTaskThirdwebEngineMarketplaceOffersCancel::Activate()
{
	ThirdwebEngine::Marketplace::Offers::Cancel(
		this,
		Chain,
		MarketplaceContract,
		BackendWalletAddress,
		{SmartWallet},
		OfferId,
		Idempotency,
		bSimulateTransaction,
		BIND_UOBJECT_DELEGATE(FStringDelegate, HandleResponse),
		BIND_UOBJECT_DELEGATE(FStringDelegate, HandleFailed)
	);
}
