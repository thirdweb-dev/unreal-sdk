// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "ThirdwebMacros.h"
#include "Engine/ThirdwebAsset.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ThirdwebMarketplaceSubsystem.generated.h"

struct FThirdwebMarketplaceBid;
struct FThirdwebMarketplaceEnglishAuction;
class UThirdwebMarketplace;

UCLASS()
class THIRDWEB_API UThirdwebMarketplaceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	/** Overrides */
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable, meta=(WorldContext="WorldContextObject"), Category="Thirdweb|Marketplace")
	static UThirdwebMarketplace* AddMarketplace(const UObject* WorldContextObject, const int64 ChainId, const FString& Address);

	UFUNCTION(BlueprintCallable, meta=(WorldContext="WorldContextObject"), Category="Thirdweb|Marketplace")
	static UThirdwebMarketplace* RegisterMarketplace(const UObject* WorldContextObject, UThirdwebMarketplace* Marketplace);

	UFUNCTION(BlueprintCallable, meta=(WorldContext="WorldContextObject"), Category="Thirdweb|Marketplace")
	static void DeregisterMarketplace(const UObject* WorldContextObject, UThirdwebMarketplace* Marketplace);

	UFUNCTION(BlueprintPure, meta=(WorldContext="WorldContextObject"), Category="Thirdweb|Marketplace")
	static UThirdwebMarketplace* FindMarketplace(const UObject* WorldContextObject, const int64 ChainId, const FString& Address);

	UFUNCTION(BlueprintPure, meta=(WorldContext="WorldContextObject"), Category="Thirdweb|Marketplace")
	static TArray<UThirdwebMarketplace*> GetAllMarketplaces(const UObject* WorldContextObject);

	static UThirdwebMarketplaceSubsystem* Get(const UObject* WorldContextObject);

protected:
	//DECLARE_DELEGATE_OneParam(FGetAllDelegate, const TArray<FThirdwebMarketplaceEnglishAuction>& /* Auctions */)
	//void GetAll(
	//	const int32 Count,
	//	const FString& Seller,
	//	const int32 Start,
	//	const FString& TokenContract,
	//	const FString& TokenId,
	//	const bool bOnlyValid,
	//	const FGetAllDelegate& SuccessDelegate,
	//	const FStringDelegate& ErrorDelegate
	//);
//
	//DECLARE_DELEGATE_OneParam(FGetAuctionDelegate, const FThirdwebMarketplaceEnglishAuction& /* Listing */)
	//void Get(
	//	const FString& ListingId,
	//	const FGetAuctionDelegate& SuccessDelegate,
	//	const FStringDelegate& ErrorDelegate
	//);
//
	//void GetBidBufferBps(
	//	const FString& ListingId,
	//	const FInt32Delegate& SuccessDelegate,
	//	const FStringDelegate& ErrorDelegate
	//);
//
	//DECLARE_DELEGATE_OneParam(FGetCurrencyValueDelegate, const FThirdwebAssetCurrencyValue& /* Value */)
	//void GetMinimumNextBid(
	//	const FString& ListingId,
	//	const FStringDelegate& SuccessDelegate,
	//	const FStringDelegate& ErrorDelegate
	//);
//
	//DECLARE_DELEGATE_OneParam(FGetBidDelegate, const FThirdwebMarketplaceBid& /* Bid */)
	//void GetWinningBid(
	//	const FString& ListingId,
	//	const FGetBidDelegate& SuccessDelegate,
	//	const FStringDelegate& ErrorDelegate
	//);
//
	//void GetTotalCount(
	//	const FStringDelegate& SuccessDelegate,
	//	const FStringDelegate& ErrorDelegate
	//);
//
	//void IsWinningBid(
	//	const FString& ListingId,
	//	const FString& BidAmount,
	//	const FBoolDelegate& SuccessDelegate,
	//	const FStringDelegate& ErrorDelegate
	//);
//
	//void GetWinner(
	//	const FString& ListingId,
	//	const FStringDelegate& SuccessDelegate,
	//	const FStringDelegate& ErrorDelegate
	//);
//
	//void Buyout(
	//	const FString& BackendWalletAddress,
	//	const FThirdwebAccountIdentifierParams& Account,
	//	const FString& ListingId,
	//	const FString& IdempotencyKey,
	//	const bool bSimulateTx,
	//	const FStringDelegate& SuccessDelegate,
	//	const FStringDelegate& ErrorDelegate
	//);
	//void Cancel(
	//	const FString& BackendWalletAddress,
	//	const FThirdwebAccountIdentifierParams& Account,
	//	const FString& ListingId,
	//	const FString& IdempotencyKey,
	//	const bool bSimulateTx,
	//	const FStringDelegate& SuccessDelegate,
	//	const FStringDelegate& ErrorDelegate
	//);
	//void Create(
	//	const FString& BackendWalletAddress,
	//	const FThirdwebAccountIdentifierParams& Account,
	//	const FThirdwebMarketplaceCreateEnglishAuctionRequest& EnglishAuctionRequest,
	//	const FString& IdempotencyKey,
	//	const bool bSimulateTx,
	//	const FStringDelegate& SuccessDelegate,
	//	const FStringDelegate& ErrorDelegate
	//);
	//void CloseForBidder(
	//	const FString& BackendWalletAddress,
	//	const FThirdwebAccountIdentifierParams& Account,
	//	const FString& ListingId,
	//	const FString& IdempotencyKey,
	//	const bool bSimulateTx,
	//	const FStringDelegate& SuccessDelegate,
	//	const FStringDelegate& ErrorDelegate
	//);
	//void CloseForSeller(
	//	const FString& BackendWalletAddress,
	//	const FThirdwebAccountIdentifierParams& Account,
	//	const FString& ListingId,
	//	const FString& IdempotencyKey,
	//	const bool bSimulateTx,
	//	const FStringDelegate& SuccessDelegate,
	//	const FStringDelegate& ErrorDelegate
	//);
	//void ExecuteSale(
	//	const FString& BackendWalletAddress,
	//	const FThirdwebAccountIdentifierParams& Account,
	//	const FString& ListingId,
	//	const FString& IdempotencyKey,
	//	const bool bSimulateTx,
	//	const FStringDelegate& SuccessDelegate,
	//	const FStringDelegate& ErrorDelegate
	//);
	//void Bid(
	//	const FString& BackendWalletAddress,
	//	const FThirdwebAccountIdentifierParams& Account,
	//	const FString& ListingId,
	//	const FString& BidAmount,
	//	const FString& IdempotencyKey,
	//	const bool bSimulateTx,
	//	const FStringDelegate& SuccessDelegate,
	//	const FStringDelegate& ErrorDelegate
	//);

private:
	/** ChainId -> Address -> Instance */
	TMap<int64, TMap<FString, UThirdwebMarketplace*>> Marketplaces;
};
