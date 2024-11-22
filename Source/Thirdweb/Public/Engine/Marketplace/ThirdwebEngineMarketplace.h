// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "ThirdwebMacros.h"

struct FThirdwebMarketplaceListing;
struct FThirdwebMarketplaceMakeOfferRequest;
struct FThirdwebMarketplaceCreateAuctionRequest;
struct FThirdwebMarketplaceCreateListingRequest;
struct FThirdwebEngineTransactionOverrides;
struct FSmartWalletHandle;
struct FThirdwebEngineTransactionStatusResult;
struct FThirdwebEngineTransactionReceipt;

namespace ThirdwebEngine::Marketplace
{
	extern FString FormatUrl(const int64 ChainId, const FString& ContractAddress, const TCHAR* InFormatString, const FStringFormatOrderedArguments& InOrderedArguments);

	namespace DirectListings
	{
		extern FString FormatUrl(const int64 ChainId, const FString& ContractAddress, const TCHAR* InFormatString);

		extern FString FormatUrl(const int64 ChainId, const FString& ContractAddress, const TCHAR* InFormatString, const FStringFormatOrderedArguments& InOrderedArguments);

		DECLARE_DELEGATE_OneParam(FGetAllDelegate, const TArray<FThirdwebMarketplaceListing>& /* Listings */)
		extern THIRDWEB_API void GetAll(
			const UObject* Outer,
			const int32 Count,
			const FString& Seller,
			const int32 Start,
			const FString& TokenContract,
			const FString& TokenId,
			const int64 Chain,
			const FString& ContractAddress,
			const FGetAllDelegate& SuccessDelegate,
			const FStringDelegate& ErrorDelegate
		);
		
		extern THIRDWEB_API void GetAllValid(
			const UObject* Outer,
			const int32 Count,
			const FString& Seller,
			const int32 Start,
			const FString& TokenContract,
			const FString& TokenId,
			const int64 Chain,
			const FString& ContractAddress,
			const FGetAllDelegate& SuccessDelegate,
			const FStringDelegate& ErrorDelegate
		);
		
		DECLARE_DELEGATE_OneParam(FGetListingDelegate, const FThirdwebMarketplaceListing& /* Listing */)
		extern THIRDWEB_API void GetListing(
			const UObject* Outer,
			const FString& ListingId,
			const int64 Chain,
			const FString& ContractAddress,
			const FGetListingDelegate& SuccessDelegate,
			const FStringDelegate& ErrorDelegate
		);
		extern THIRDWEB_API void IsBuyerApprovedForListing(
			const UObject* Outer,
			const FString& ListingId,
			const FString& WalletAddress,
			const int64 Chain,
			const FString& ContractAddress,
			const FBoolDelegate& SuccessDelegate,
			const FStringDelegate& ErrorDelegate
		);
		extern THIRDWEB_API void IsCurrencyApprovedForListing(
			const UObject* Outer,
			const FString& ListingId,
			const FString& CurrencyContractAddress,
			const int64 Chain,
			const FString& ContractAddress,
			const FBoolDelegate& SuccessDelegate,
			const FStringDelegate& ErrorDelegate
		);
		extern THIRDWEB_API void GetTotalCount(
			const UObject* Outer,
			const int64 Chain,
			const FString& ContractAddress,
			const FStringDelegate& SuccessDelegate,
			const FStringDelegate& ErrorDelegate
		);

#define DIRECT_LISTING_POST_PARAMS const bool bSimulateTx, const int64 Chain, const FString& ContractAddress, const FString& BackendWalletAddress, const FString& IdempotencyKey, const FString& AccountAddress, const FString& AccountFactoryAddress, const FString& AccountSalt

		extern THIRDWEB_API void CreateListing(
			const UObject* Outer,
			DIRECT_LISTING_POST_PARAMS,
			const FThirdwebMarketplaceCreateListingRequest& Request,
			const FStringDelegate& SuccessDelegate,
			const FStringDelegate& ErrorDelegate
		);
		extern THIRDWEB_API void UpdateListing(
			const UObject* Outer,
			DIRECT_LISTING_POST_PARAMS,
			const FString& ListingId,
			const FStringDelegate& SuccessDelegate,
			const FStringDelegate& ErrorDelegate
		);
		extern THIRDWEB_API void BuyFromListing(
			const UObject* Outer,
			DIRECT_LISTING_POST_PARAMS,
			const FString& ListingId,
			const FString& Quantity,
			const FString& Buyer,
			const FStringDelegate& SuccessDelegate,
			const FStringDelegate& ErrorDelegate
		);
		extern THIRDWEB_API void ApproveBuyerForReservedListing(
			const UObject* Outer,
			DIRECT_LISTING_POST_PARAMS,
			const FString& ListingId,
			const FString& Buyer,
			const FStringDelegate& SuccessDelegate,
			const FStringDelegate& ErrorDelegate
		);
		extern THIRDWEB_API void RevokeBuyerApprovalForReservedListing(
			const UObject* Outer,
			DIRECT_LISTING_POST_PARAMS,
			const FString& ListingId,
			const FString& BuyerAddress,
			const FStringDelegate& SuccessDelegate,
			const FStringDelegate& ErrorDelegate
		);
		extern THIRDWEB_API void RevokeCurrencyApprovalForListing(
			const UObject* Outer,
			DIRECT_LISTING_POST_PARAMS,
			const FString& ListingId,
			const FString& CurrencyContractAddress,
			const FStringDelegate& SuccessDelegate,
			const FStringDelegate& ErrorDelegate
		);
		extern THIRDWEB_API void CancelListing(
			const UObject* Outer,
			DIRECT_LISTING_POST_PARAMS,
			const FString& ListingId,
			const FStringDelegate& SuccessDelegate,
			const FStringDelegate& ErrorDelegate
		);
	}

	namespace EnglishAuctions
	{
		extern THIRDWEB_API void GetAll(
			const UObject* Outer,
			const int32 Count,
			const FString& Seller,
			const int32 Start,
			const FString& TokenContract,
			const FString& TokenId,
			const int64 Chain,
			const FString& ContractAddress,
			const FStringDelegate& SuccessDelegate,
			const FStringDelegate& ErrorDelegate
		);
		extern THIRDWEB_API void GetAllValid(
			const UObject* Outer,
			const int32 Count,
			const FString& Seller,
			const int32 Start,
			const FString& TokenContract,
			const FString& TokenId,
			const int64 Chain,
			const FString& ContractAddress,
			const FStringDelegate& SuccessDelegate,
			const FStringDelegate& ErrorDelegate
		);
		extern THIRDWEB_API void GetAuction(
			const UObject* Outer,
			const FString& ListingId,
			const int64 Chain,
			const FString& ContractAddress,
			const FStringDelegate& SuccessDelegate,
			const FStringDelegate& ErrorDelegate
		);
		extern THIRDWEB_API void GetBidBufferBps(
			const UObject* Outer,
			const FString& ListingId,
			const int64 Chain,
			const FString& ContractAddress,
			const FStringDelegate& SuccessDelegate,
			const FStringDelegate& ErrorDelegate
		);
		extern THIRDWEB_API void GetMinimumNextBid(
			const UObject* Outer,
			const FString& ListingId,
			const int64 Chain,
			const FString& ContractAddress,
			const FStringDelegate& SuccessDelegate,
			const FStringDelegate& ErrorDelegate
		);
		extern THIRDWEB_API void GetWinningBid(
			const UObject* Outer,
			const FString& ListingId,
			const int64 Chain,
			const FString& ContractAddress,
			const FStringDelegate& SuccessDelegate,
			const FStringDelegate& ErrorDelegate
		);
		extern THIRDWEB_API void GetTotalCount(
			const UObject* Outer,
			const int64 Chain,
			const FString& ContractAddress,
			const FStringDelegate& SuccessDelegate,
			const FStringDelegate& ErrorDelegate
		);
		extern THIRDWEB_API void IsWinningBid(
			const UObject* Outer,
			const FString& ListingId,
			const FString& BidAmount,
			const int64 Chain,
			const FString& ContractAddress,
			const FStringDelegate& SuccessDelegate,
			const FStringDelegate& ErrorDelegate
		);
		extern THIRDWEB_API void GetWinner(
			const UObject* Outer,
			const FString& ListingId,
			const int64 Chain,
			const FString& ContractAddress,
			const FStringDelegate& SuccessDelegate,
			const FStringDelegate& ErrorDelegate
		);

		extern THIRDWEB_API void BuyoutAuction(
			const UObject* Outer,
			const bool bSimulateTx,
			const FString& ListingId,
			const int64 Chain,
			const FString& ContractAddress,
			const FStringDelegate& SuccessDelegate,
			const FStringDelegate& ErrorDelegate
		);
		extern THIRDWEB_API void CancelAuction(
			const UObject* Outer,
			const bool bSimulateTx,
			const FString& ListingId,
			const int64 Chain,
			const FString& ContractAddress,
			const FStringDelegate& SuccessDelegate,
			const FStringDelegate& ErrorDelegate
		);
		extern THIRDWEB_API void CreateAuction(
			const UObject* Outer,
			const bool bSimulateTx,
			const FThirdwebMarketplaceCreateAuctionRequest& Request,
			const int64 Chain,
			const FString& ContractAddress,
			const FStringDelegate& SuccessDelegate,
			const FStringDelegate& ErrorDelegate
		);
		extern THIRDWEB_API void CloseAuctionForBidder(
			const UObject* Outer,
			const bool bSimulateTx,
			const FString& ListingId,
			const int64 Chain,
			const FString& ContractAddress,
			const FStringDelegate& SuccessDelegate,
			const FStringDelegate& ErrorDelegate
		);
		extern THIRDWEB_API void CloseAuctionForSeller(
			const UObject* Outer,
			const bool bSimulateTx,
			const FString& ListingId,
			const int64 Chain,
			const FString& ContractAddress,
			const FStringDelegate& SuccessDelegate,
			const FStringDelegate& ErrorDelegate
		);
		extern THIRDWEB_API void ExecuteSale(
			const UObject* Outer,
			const bool bSimulateTx,
			const FString& ListingId,
			const int64 Chain,
			const FString& ContractAddress,
			const FStringDelegate& SuccessDelegate,
			const FStringDelegate& ErrorDelegate
		);
		extern THIRDWEB_API void MakeBid(
			const UObject* Outer,
			const bool bSimulateTx,
			const FString& ListingId,
			const FString& BidAmount,
			const int64 Chain,
			const FString& ContractAddress,
			const FStringDelegate& SuccessDelegate,
			const FStringDelegate& ErrorDelegate
		);
	}

	namespace Offers
	{
		extern THIRDWEB_API void GetAll(
			const UObject* Outer,
			const int32 Count,
			const FString& Offeror,
			const int32 Start,
			const FString& TokenContract,
			const FString& TokenId,
			const int64 Chain,
			const FString& ContractAddress,
			const FStringDelegate& SuccessDelegate,
			const FStringDelegate& ErrorDelegate
		);
		extern THIRDWEB_API void GetAllValid(
			const UObject* Outer,
			const int32 Count,
			const FString& Offeror,
			const int32 Start,
			const FString& TokenContract,
			const FString& TokenId,
			const int64 Chain,
			const FString& ContractAddress,
			const FStringDelegate& SuccessDelegate,
			const FStringDelegate& ErrorDelegate
		);
		extern THIRDWEB_API void GetOffer(const UObject* Outer, const FString& OfferId, const FStringDelegate& SuccessDelegate, const FStringDelegate& ErrorDelegate);
		extern THIRDWEB_API void GetTotalCount(const UObject* Outer, const FStringDelegate& SuccessDelegate, const FStringDelegate& ErrorDelegate);
		extern THIRDWEB_API void MakeOffer(
			const UObject* Outer,
			DIRECT_LISTING_POST_PARAMS,
			const FThirdwebMarketplaceMakeOfferRequest& Request,
			const FStringDelegate& SuccessDelegate,
			const FStringDelegate& ErrorDelegate
		);
		extern THIRDWEB_API void CancelOffer(
			const UObject* Outer,
			DIRECT_LISTING_POST_PARAMS,
			const FString& OfferId,
			const FStringDelegate& SuccessDelegate,
			const FStringDelegate& ErrorDelegate
		);
		extern THIRDWEB_API void AcceptOffer(
			const UObject* Outer,
			DIRECT_LISTING_POST_PARAMS,
			const FString& OfferId,
			const FStringDelegate& SuccessDelegate,
			const FStringDelegate& ErrorDelegate
		);
	}
}
