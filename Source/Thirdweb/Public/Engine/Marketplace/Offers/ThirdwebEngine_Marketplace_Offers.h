// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "ThirdwebMacros.h"

struct FThirdwebMarketplaceMakeOfferRequest;
struct FThirdwebAccountIdentifierParams;
struct FThirdwebURLSearchParams;
struct FThirdwebMarketplaceOffer;

namespace ThirdwebEngine::Marketplace::Offers
{
	extern FString FormatUrl(const int64 ChainId, const FString& ContractAddress, const FString& Endpoint, const FThirdwebURLSearchParams& Params);

	DECLARE_DELEGATE_OneParam(FGetAllDelegate, const TArray<FThirdwebMarketplaceOffer>& /* Offers */)
	extern THIRDWEB_API void GetAll(
		const UObject* Outer,
		const int32 Count,
		const FString& Offeror,
		const int32 Start,
		const FString& TokenContract,
		const FString& TokenId,
		const int64 Chain,
		const FString& ContractAddress,
		const bool bOnlyValid,
		const FGetAllDelegate& SuccessDelegate,
		const FStringDelegate& ErrorDelegate
	);

	DECLARE_DELEGATE_OneParam(FGetOfferDelegate, const FThirdwebMarketplaceOffer& /* Offer */)
	extern THIRDWEB_API void Get(
		const UObject* Outer,
		const FString& OfferId,
		const int64 Chain,
		const FString& ContractAddress,
		const FGetOfferDelegate& SuccessDelegate,
		const FStringDelegate& ErrorDelegate
	);

	extern THIRDWEB_API void GetTotalCount(
		const UObject* Outer,
		const int64 Chain,
		const FString& ContractAddress,
		const FStringDelegate& SuccessDelegate,
		const FStringDelegate& ErrorDelegate
	);

	extern THIRDWEB_API void Offer(
		const UObject* Outer,
		const int64 Chain,
		const FString& ContractAddress,
		const FString& BackendWalletAddress,
		const FThirdwebAccountIdentifierParams& Account,
		const FThirdwebMarketplaceMakeOfferRequest& OfferRequest,
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
		const FString& OfferId,
		const FString& IdempotencyKey,
		const bool bSimulateTx,
		const FStringDelegate& SuccessDelegate,
		const FStringDelegate& ErrorDelegate
	);
	
	extern THIRDWEB_API void Accept(
		const UObject* Outer,
		const int64 Chain,
		const FString& ContractAddress,
		const FString& BackendWalletAddress,
		const FThirdwebAccountIdentifierParams& Account,
		const FString& OfferId,
		const FString& IdempotencyKey,
		const bool bSimulateTx,
		const FStringDelegate& SuccessDelegate,
		const FStringDelegate& ErrorDelegate
	);
}
