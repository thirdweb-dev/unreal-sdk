// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "Engine/Marketplace/DirectListings/ThirdwebEngine_Marketplace_DirectListings.h"

#include "ThirdwebLog.h"
#include "ThirdwebUtils.h"
#include "Engine/Marketplace/ThirdwebEngine_Marketplace.h"
#include "Engine/Marketplace/DirectListings/ThirdwebMarketplaceListing.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Internal/ThirdwebHeaders.h"
#include "Internal/ThirdwebURLSearchParams.h"
#include "Wallets/ThirdwebSmartWalletHandle.h"

namespace ThirdwebEngine::Marketplace::DirectListings
{
	FString FormatUrl(const int64 ChainId, const FString& ContractAddress, const FString& Endpoint, const FThirdwebURLSearchParams& Params)
	{
		return Marketplace::FormatUrl(ChainId, ContractAddress, TEXT("direct-listings"), Endpoint, Params);
	}

	void GetAll(
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
	)
	{
		const TSharedRef<IHttpRequest> Request = ThirdwebUtils::Internal::CreateEngineRequest();

		FThirdwebURLSearchParams Params;
		Params.Set(TEXT("count"), Count, Count > 0);
		Params.Set(TEXT("seller"), Seller, !Seller.IsEmpty());
		Params.Set(TEXT("start"), Start, Start >= 0);
		Params.Set(TEXT("tokenContract"), TokenContract, !TokenContract.IsEmpty());
		Params.Set(TEXT("tokenId"), TokenContract, TokenId.IsNumeric() && !TokenId.StartsWith("-"));

		Request->SetURL(FormatUrl(Chain, ContractAddress, TEXT("get-all"), Params));
		ThirdwebUtils::Internal::LogRequest(Request);
		Request->OnProcessRequestComplete().BindWeakLambda(Outer, [SuccessDelegate, ErrorDelegate](HTTP_LAMBDA_PARAMS)
		{
			CHECK_NETWORK
			FString Content = Response->GetContentAsString();
			TW_LOG(Verbose, TEXT("ThirdwebEngine::Marketplace::DirectListings::GetAll::Content=%s"), *Content)
			FString Error;
			if (TArray<TSharedPtr<FJsonValue>> JsonArray; ThirdwebUtils::Json::ParseEngineResponse(Content, JsonArray, Error))
			{
				EXECUTE_IF_BOUND(SuccessDelegate, FThirdwebMarketplaceListing::FromJson(JsonArray))
			}
			else
			{
				EXECUTE_IF_BOUND(ErrorDelegate, Error)
			}
		});
		Request->ProcessRequest();
	}

	void GetAllValid(
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
	)
	{
		const TSharedRef<IHttpRequest> Request = ThirdwebUtils::Internal::CreateEngineRequest();

		FThirdwebURLSearchParams Params;
		Params.Set(TEXT("count"), Count, Count > 0);
		Params.Set(TEXT("seller"), Seller, !Seller.IsEmpty());
		Params.Set(TEXT("start"), Start, Start >= 0);
		Params.Set(TEXT("tokenContract"), TokenContract, !TokenContract.IsEmpty());
		Params.Set(TEXT("tokenId"), TokenContract, TokenId.IsNumeric() && !TokenId.StartsWith("-"));

		Request->SetURL(FormatUrl(Chain, ContractAddress, TEXT("get-all-valid"), Params));

		ThirdwebUtils::Internal::LogRequest(Request);
		Request->OnProcessRequestComplete().BindWeakLambda(Outer, [SuccessDelegate, ErrorDelegate](HTTP_LAMBDA_PARAMS)
		{
			CHECK_NETWORK
			FString Content = Response->GetContentAsString();
			TW_LOG(Verbose, TEXT("ThirdwebEngine::Marketplace::DirectListings::GetAllValid::Content=%s"), *Content)
			FString Error;
			if (TArray<TSharedPtr<FJsonValue>> JsonArray; ThirdwebUtils::Json::ParseEngineResponse(Content, JsonArray, Error))
			{
				EXECUTE_IF_BOUND(SuccessDelegate, FThirdwebMarketplaceListing::FromJson(JsonArray))
			}
			else
			{
				EXECUTE_IF_BOUND(ErrorDelegate, Error)
			}
		});
		Request->ProcessRequest();
	}

	void GetListing(
		const UObject* Outer,
		const FString& ListingId,
		const int64 Chain,
		const FString& ContractAddress,
		const FGetListingDelegate& SuccessDelegate,
		const FStringDelegate& ErrorDelegate
	)
	{
		const TSharedRef<IHttpRequest> Request = ThirdwebUtils::Internal::CreateEngineRequest();

		FThirdwebURLSearchParams Params;
		Params.Set(TEXT("listingId"), ListingId, ListingId.IsNumeric() && !ListingId.StartsWith("-"));

		Request->SetURL(FormatUrl(Chain, ContractAddress, TEXT("get-listing"), Params));

		ThirdwebUtils::Internal::LogRequest(Request);
		Request->OnProcessRequestComplete().BindWeakLambda(Outer, [SuccessDelegate, ErrorDelegate](HTTP_LAMBDA_PARAMS)
		{
			CHECK_NETWORK
			FString Content = Response->GetContentAsString();
			TW_LOG(Verbose, TEXT("ThirdwebEngine::Marketplace::DirectListings::GetListing::Content=%s"), *Content)
			FString Error;
			if (TSharedPtr<FJsonObject> JsonObject; ThirdwebUtils::Json::ParseEngineResponse(Content, JsonObject, Error))
			{
				EXECUTE_IF_BOUND(SuccessDelegate, FThirdwebMarketplaceListing::FromJson(JsonObject))
			}
			else
			{
				EXECUTE_IF_BOUND(ErrorDelegate, Error)
			}
		});
		Request->ProcessRequest();
	}

	void IsBuyerApprovedForListing(
		const UObject* Outer,
		const FString& ListingId,
		const FString& WalletAddress,
		const int64 Chain,
		const FString& ContractAddress,
		const FBoolDelegate& SuccessDelegate,
		const FStringDelegate& ErrorDelegate
	)
	{
		const TSharedRef<IHttpRequest> Request = ThirdwebUtils::Internal::CreateEngineRequest();

		FThirdwebURLSearchParams Params;
		Params.Set(TEXT("listingId"), ListingId, ListingId.IsNumeric() && !ListingId.StartsWith("-"));
		Params.Set(TEXT("walletAddress"), WalletAddress, !WalletAddress.IsEmpty());

		Request->SetURL(FormatUrl(Chain, ContractAddress, TEXT("is-buyer-approved-for-listing"), Params));

		ThirdwebUtils::Internal::LogRequest(Request);
		Request->OnProcessRequestComplete().BindWeakLambda(Outer, [SuccessDelegate, ErrorDelegate](HTTP_LAMBDA_PARAMS)
		{
			CHECK_NETWORK
			FString Content = Response->GetContentAsString();
			TW_LOG(Verbose, TEXT("ThirdwebEngine::Marketplace::DirectListings::IsBuyerApprovedForListing::Content=%s"), *Content)
			FString Error;
			if (TSharedPtr<FJsonValue> JsonValue; ThirdwebUtils::Json::ParseEngineResponse(Content, JsonValue, Error))
			{
				EXECUTE_IF_BOUND(SuccessDelegate, JsonValue->AsBool())
			}
			else
			{
				EXECUTE_IF_BOUND(ErrorDelegate, Error)
			}
		});
		Request->ProcessRequest();
	}

	void IsCurrencyApprovedForListing(
		const UObject* Outer,
		const FString& ListingId,
		const FString& CurrencyContractAddress,
		const int64 Chain,
		const FString& ContractAddress,
		const FBoolDelegate& SuccessDelegate,
		const FStringDelegate& ErrorDelegate
	)
	{
		const TSharedRef<IHttpRequest> Request = ThirdwebUtils::Internal::CreateEngineRequest();

		FThirdwebURLSearchParams Params;
		Params.Set(TEXT("listingId"), ListingId, ListingId.IsNumeric() && !ListingId.StartsWith("-"));
		Params.Set(TEXT("currencyContractAddress"), CurrencyContractAddress, !CurrencyContractAddress.IsEmpty());

		Request->SetURL(FormatUrl(Chain, ContractAddress, TEXT("is-currency-approved-for-listing"), Params));

		ThirdwebUtils::Internal::LogRequest(Request);
		Request->OnProcessRequestComplete().BindWeakLambda(Outer, [SuccessDelegate, ErrorDelegate](HTTP_LAMBDA_PARAMS)
		{
			CHECK_NETWORK
			FString Content = Response->GetContentAsString();
			TW_LOG(Verbose, TEXT("ThirdwebEngine::Marketplace::DirectListings::IsCurrencyApprovedForListing::Content=%s"), *Content)
			FString Error;
			if (TSharedPtr<FJsonValue> JsonValue; ThirdwebUtils::Json::ParseEngineResponse(Content, JsonValue, Error))
			{
				EXECUTE_IF_BOUND(SuccessDelegate, JsonValue->AsBool())
			}
			else
			{
				EXECUTE_IF_BOUND(ErrorDelegate, Error)
			}
		});
		Request->ProcessRequest();
	}

	void GetTotalCount(const UObject* Outer, const int64 Chain, const FString& ContractAddress, const FStringDelegate& SuccessDelegate, const FStringDelegate& ErrorDelegate)
	{
		const TSharedRef<IHttpRequest> Request = ThirdwebUtils::Internal::CreateEngineRequest();

		Request->SetURL(FormatUrl(Chain, ContractAddress, TEXT("get-total-count"), {}));

		ThirdwebUtils::Internal::LogRequest(Request);
		Request->OnProcessRequestComplete().BindWeakLambda(Outer, [SuccessDelegate, ErrorDelegate](HTTP_LAMBDA_PARAMS)
		{
			CHECK_NETWORK
			FString Content = Response->GetContentAsString();
			TW_LOG(Verbose, TEXT("ThirdwebEngine::Marketplace::DirectListings::GetTotalCount::Content=%s"), *Content)
			FString Error;
			if (TSharedPtr<FJsonValue> JsonValue; ThirdwebUtils::Json::ParseEngineResponse(Content, JsonValue, Error))
			{
				EXECUTE_IF_BOUND(SuccessDelegate, JsonValue->AsString())
			}
			else
			{
				EXECUTE_IF_BOUND(ErrorDelegate, Error)
			}
		});
		Request->ProcessRequest();
	}

	void CreateListing(
		const UObject* Outer,
		const int64 Chain,
		const FString& ContractAddress,
		const FString& BackendWalletAddress,
		const FThirdwebAccountIdentifierParams& Account,
		const FThirdwebMarketplaceCreateListingRequest& CreateListingRequest,
		const FString& IdempotencyKey,
		const bool bSimulateTx,
		const FStringDelegate& SuccessDelegate,
		const FStringDelegate& ErrorDelegate
	)
	{
		const TSharedRef<IHttpRequest> Request = ThirdwebUtils::Internal::CreateEngineRequest(TEXT("POST"));
		FThirdwebURLSearchParams Params;
		Params.Set(TEXT("simulateTx"), true, bSimulateTx);

		Request->SetURL(FormatUrl(Chain, ContractAddress, TEXT("create-listing"), Params));

		MakePostHeaders(BackendWalletAddress, IdempotencyKey, Account).UpdateRequest(Request);

		Request->SetContentAsString(ThirdwebUtils::Json::ToString(CreateListingRequest.ToJson()));

		ThirdwebUtils::Internal::LogRequest(Request);
		Request->OnProcessRequestComplete().BindWeakLambda(Outer, [SuccessDelegate, ErrorDelegate](HTTP_LAMBDA_PARAMS)
		{
			CHECK_NETWORK
			FString Content = Response->GetContentAsString();
			TW_LOG(Verbose, TEXT("ThirdwebEngine::Marketplace::DirectListings::CreateListing::Content=%s"), *Content)
			HANDLE_QUEUE_ID_RESPONSE
		});
		Request->ProcessRequest();
	}

	void UpdateListing(
		const UObject* Outer,
		const int64 Chain,
		const FString& ContractAddress,
		const FString& BackendWalletAddress,
		const FThirdwebAccountIdentifierParams& Account,
		const FThirdwebMarketplaceUpdateListingRequest& UpdateListingRequest,
		const FString& IdempotencyKey,
		const bool bSimulateTx,
		const FStringDelegate& SuccessDelegate,
		const FStringDelegate& ErrorDelegate
	)
	{
		const TSharedRef<IHttpRequest> Request = ThirdwebUtils::Internal::CreateEngineRequest(TEXT("POST"));
		FThirdwebURLSearchParams Params;
		Params.Set(TEXT("simulateTx"), true, bSimulateTx);

		Request->SetURL(FormatUrl(Chain, ContractAddress, TEXT("update-listing"), Params));

		MakePostHeaders(BackendWalletAddress, IdempotencyKey, Account).UpdateRequest(Request);

		Request->SetContentAsString(ThirdwebUtils::Json::ToString(UpdateListingRequest.ToJson()));

		ThirdwebUtils::Internal::LogRequest(Request);
		Request->OnProcessRequestComplete().BindWeakLambda(Outer, [SuccessDelegate, ErrorDelegate](HTTP_LAMBDA_PARAMS)
		{
			CHECK_NETWORK
			FString Content = Response->GetContentAsString();
			TW_LOG(Verbose, TEXT("ThirdwebEngine::Marketplace::DirectListings::UpdateListing::Content=%s"), *Content)
			HANDLE_QUEUE_ID_RESPONSE
		});
		Request->ProcessRequest();
	}

	void BuyFromListing(
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
	)
	{
		const TSharedRef<IHttpRequest> Request = ThirdwebUtils::Internal::CreateEngineRequest(TEXT("POST"));
		FThirdwebURLSearchParams Params;
		Params.Set(TEXT("simulateTx"), true, bSimulateTx);

		Request->SetURL(FormatUrl(Chain, ContractAddress, TEXT("buy-from-listing"), Params));

		MakePostHeaders(BackendWalletAddress, IdempotencyKey, Account).UpdateRequest(Request);

		TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
		JsonObject->SetStringField(TEXT("listingId"), ListingId);
		JsonObject->SetStringField(TEXT("quantity"), Quantity);
		JsonObject->SetStringField(TEXT("buyer"), Buyer);
		Request->SetContentAsString(ThirdwebUtils::Json::ToString(JsonObject));

		ThirdwebUtils::Internal::LogRequest(Request);
		Request->OnProcessRequestComplete().BindWeakLambda(Outer, [SuccessDelegate, ErrorDelegate](HTTP_LAMBDA_PARAMS)
		{
			CHECK_NETWORK
			FString Content = Response->GetContentAsString();
			TW_LOG(Verbose, TEXT("ThirdwebEngine::Marketplace::DirectListings::BuyFromListing::Content=%s"), *Content)
			HANDLE_QUEUE_ID_RESPONSE
		});
		Request->ProcessRequest();
	}

	void ApproveBuyerForReservedListing(
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
	)
	{
		const TSharedRef<IHttpRequest> Request = ThirdwebUtils::Internal::CreateEngineRequest(TEXT("POST"));
		FThirdwebURLSearchParams Params;
		Params.Set(TEXT("simulateTx"), true, bSimulateTx);

		Request->SetURL(FormatUrl(Chain, ContractAddress, TEXT("approve-buyer-for-reserved-listing"), Params));

		MakePostHeaders(BackendWalletAddress, IdempotencyKey, Account).UpdateRequest(Request);

		TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
		JsonObject->SetStringField(TEXT("listingId"), ListingId);
		JsonObject->SetStringField(TEXT("buyer"), Buyer);
		Request->SetContentAsString(ThirdwebUtils::Json::ToString(JsonObject));

		ThirdwebUtils::Internal::LogRequest(Request);
		Request->OnProcessRequestComplete().BindWeakLambda(Outer, [SuccessDelegate, ErrorDelegate](HTTP_LAMBDA_PARAMS)
		{
			CHECK_NETWORK
			FString Content = Response->GetContentAsString();
			TW_LOG(Verbose, TEXT("ThirdwebEngine::Marketplace::DirectListings::ApproveBuyerForReservedListing::Content=%s"), *Content)
			HANDLE_QUEUE_ID_RESPONSE
		});
		Request->ProcessRequest();
	}

	void RevokeBuyerApprovalForReservedListing(
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
	)
	{
		const TSharedRef<IHttpRequest> Request = ThirdwebUtils::Internal::CreateEngineRequest(TEXT("POST"));
		FThirdwebURLSearchParams Params;
		Params.Set(TEXT("simulateTx"), true, bSimulateTx);

		Request->SetURL(FormatUrl(Chain, ContractAddress, TEXT("revoke-buyer-approval-for-reserved-listing"), Params));

		MakePostHeaders(BackendWalletAddress, IdempotencyKey, Account).UpdateRequest(Request);

		TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
		JsonObject->SetStringField(TEXT("listingId"), ListingId);
		JsonObject->SetStringField(TEXT("buyerAddress"), Buyer);
		Request->SetContentAsString(ThirdwebUtils::Json::ToString(JsonObject));

		ThirdwebUtils::Internal::LogRequest(Request);
		Request->OnProcessRequestComplete().BindWeakLambda(Outer, [SuccessDelegate, ErrorDelegate](HTTP_LAMBDA_PARAMS)
		{
			CHECK_NETWORK
			FString Content = Response->GetContentAsString();
			TW_LOG(Verbose, TEXT("ThirdwebEngine::Marketplace::DirectListings::RevokeBuyerApprovalForReservedListing::Content=%s"), *Content)
			HANDLE_QUEUE_ID_RESPONSE
		});
		Request->ProcessRequest();
	}

	void RevokeCurrencyApprovalForListing(
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
	)
	{
		const TSharedRef<IHttpRequest> Request = ThirdwebUtils::Internal::CreateEngineRequest(TEXT("POST"));
		FThirdwebURLSearchParams Params;
		Params.Set(TEXT("simulateTx"), true, bSimulateTx);

		Request->SetURL(FormatUrl(Chain, ContractAddress, TEXT("revoke-currency-approval-for-listing"), Params));

		MakePostHeaders(BackendWalletAddress, IdempotencyKey, Account).UpdateRequest(Request);

		TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
		JsonObject->SetStringField(TEXT("listingId"), ListingId);
		JsonObject->SetStringField(TEXT("currencyContractAddress"), CurrencyContractAddress);
		Request->SetContentAsString(ThirdwebUtils::Json::ToString(JsonObject));

		ThirdwebUtils::Internal::LogRequest(Request);
		Request->OnProcessRequestComplete().BindWeakLambda(Outer, [SuccessDelegate, ErrorDelegate](HTTP_LAMBDA_PARAMS)
		{
			CHECK_NETWORK
			FString Content = Response->GetContentAsString();
			TW_LOG(Verbose, TEXT("ThirdwebEngine::Marketplace::DirectListings::RevokeCurrencyApprovalForListing::Content=%s"), *Content)
			HANDLE_QUEUE_ID_RESPONSE
		});
		Request->ProcessRequest();
	}

	void CancelListing(
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
	)
	{
		const TSharedRef<IHttpRequest> Request = ThirdwebUtils::Internal::CreateEngineRequest(TEXT("POST"));
		FThirdwebURLSearchParams Params;
		Params.Set(TEXT("simulateTx"), true, bSimulateTx);

		Request->SetURL(FormatUrl(Chain, ContractAddress, TEXT("cancel-listing"), Params));

		MakePostHeaders(BackendWalletAddress, IdempotencyKey, Account).UpdateRequest(Request);

		TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
		JsonObject->SetStringField(TEXT("listingId"), ListingId);
		Request->SetContentAsString(ThirdwebUtils::Json::ToString(JsonObject));

		ThirdwebUtils::Internal::LogRequest(Request);
		Request->OnProcessRequestComplete().BindWeakLambda(Outer, [SuccessDelegate, ErrorDelegate](HTTP_LAMBDA_PARAMS)
		{
			CHECK_NETWORK
			FString Content = Response->GetContentAsString();
			TW_LOG(Verbose, TEXT("ThirdwebEngine::Marketplace::DirectListings::CancelListing::Content=%s"), *Content)
			HANDLE_QUEUE_ID_RESPONSE
		});
		Request->ProcessRequest();
	}
}
