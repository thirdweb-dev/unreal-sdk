// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "ThirdwebMacros.h"

struct FThirdwebAccountIdentifierParams;
struct FThirdwebMarketplaceUpdateDirectListingRequest;
struct FThirdwebURLSearchParams;
struct FThirdwebMarketplaceCreateDirectListingRequest;
struct FThirdwebMarketplaceDirectListing;
struct FSmartWalletHandle;

namespace ThirdwebEngine::Marketplace::DirectListings
{
	extern FString FormatUrl(const int64 ChainId, const FString& ContractAddress, const FString& Endpoint, const FThirdwebURLSearchParams& Params);

	DECLARE_DELEGATE_OneParam(FGetAllDelegate, const TArray<FThirdwebMarketplaceDirectListing>& /* Listings */)
	extern THIRDWEB_API void GetAll(
		const UObject* Outer,
		const int32 Count,
		const FString& Seller,
		const int32 Start,
		const FString& TokenContract,
		const FString& TokenId,
		const int64 Chain,
		const FString& ContractAddress,
		const bool bOnlyValid,
		const FGetAllDelegate& SuccessDelegate,
		const FStringDelegate& ErrorDelegate
	);

	DECLARE_DELEGATE_OneParam(FGetListingDelegate, const FThirdwebMarketplaceDirectListing& /* Listing */)
	extern THIRDWEB_API void Get(
		const UObject* Outer,
		const FString& ListingId,
		const int64 Chain,
		const FString& ContractAddress,
		const FGetListingDelegate& SuccessDelegate,
		const FStringDelegate& ErrorDelegate
	);

	extern THIRDWEB_API void IsBuyerApproved(
		const UObject* Outer,
		const FString& ListingId,
		const FString& WalletAddress,
		const int64 Chain,
		const FString& ContractAddress,
		const FBoolDelegate& SuccessDelegate,
		const FStringDelegate& ErrorDelegate
	);

	extern THIRDWEB_API void IsCurrencyApproved(
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

	extern THIRDWEB_API void Create(
		const UObject* Outer,
		const int64 Chain,
		const FString& ContractAddress,
		const FString& BackendWalletAddress,
		const FThirdwebAccountIdentifierParams& Account,
		const FThirdwebMarketplaceCreateDirectListingRequest& CreateListingRequest,
		const FString& IdempotencyKey,
		const bool bSimulateTx,
		const FStringDelegate& SuccessDelegate,
		const FStringDelegate& ErrorDelegate
	);

	extern THIRDWEB_API void Update(
		const UObject* Outer,
		const int64 Chain,
		const FString& ContractAddress,
		const FString& BackendWalletAddress,
		const FThirdwebAccountIdentifierParams& Account,
		const FThirdwebMarketplaceUpdateDirectListingRequest& UpdateListingRequest,
		const FString& IdempotencyKey,
		const bool bSimulateTx,
		const FStringDelegate& SuccessDelegate,
		const FStringDelegate& ErrorDelegate
	);

	extern THIRDWEB_API void Buy(
		const UObject* Outer,
		const int64 Chain,
		const FString& ContractAddress,
		const FString& BackendWalletAddress,
		const FThirdwebAccountIdentifierParams& Account,
		const FString& ListingId,
		const FString& Quantity,
		const FString& Buyer,
		const FString& IdempotencyKey,
		const bool bSimulateTx,
		const FStringDelegate& SuccessDelegate,
		const FStringDelegate& ErrorDelegate
	);

	extern THIRDWEB_API void ApproveReservedBuyer(
		const UObject* Outer,
		const int64 Chain,
		const FString& ContractAddress,
		const FString& BackendWalletAddress,
		const FThirdwebAccountIdentifierParams& Account,
		const FString& ListingId,
		const FString& Buyer,
		const FString& IdempotencyKey,
		const bool bSimulateTx,
		const FStringDelegate& SuccessDelegate,
		const FStringDelegate& ErrorDelegate
	);

	extern THIRDWEB_API void RevokeReservedBuyerApproval(
		const UObject* Outer,
		const int64 Chain,
		const FString& ContractAddress,
		const FString& BackendWalletAddress,
		const FThirdwebAccountIdentifierParams& Account,
		const FString& ListingId,
		const FString& Buyer,
		const FString& IdempotencyKey,
		const bool bSimulateTx,
		const FStringDelegate& SuccessDelegate,
		const FStringDelegate& ErrorDelegate
	);

	extern THIRDWEB_API void RevokeReservedCurrencyApproval(
		const UObject* Outer,
		const int64 Chain,
		const FString& ContractAddress,
		const FString& BackendWalletAddress,
		const FThirdwebAccountIdentifierParams& Account,
		const FString& ListingId,
		const FString& CurrencyContractAddress,
		const FString& IdempotencyKey,
		const bool bSimulateTx,
		const FStringDelegate& SuccessDelegate,
		const FStringDelegate& ErrorDelegate
	);
	extern THIRDWEB_API void Cancel(
		const UObject* Outer,
		const int64 Chain,
		const FString& ContractAddress,
		const FString& BackendWalletAddress,
		const FThirdwebAccountIdentifierParams& Account,
		const FString& ListingId,
		const FString& IdempotencyKey,
		const bool bSimulateTx,
		const FStringDelegate& SuccessDelegate,
		const FStringDelegate& ErrorDelegate
	);
}
