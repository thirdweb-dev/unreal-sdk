// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "Engine/Marketplace/DirectListings/ThirdwebEngine_Marketplace_DirectListings.h"

#include "ThirdwebLog.h"
#include "ThirdwebUtils.h"
#include "Engine/Marketplace/ThirdwebEngine_Marketplace.h"
#include "Engine/Marketplace/DirectListings/ThirdwebMarketplaceDirectListing.h"
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
		const bool bOnlyValid,
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

		Request->SetURL(FormatUrl(Chain, ContractAddress, bOnlyValid ? TEXT("get-all-valid") : TEXT("get-all"), Params));
		ThirdwebUtils::Internal::LogRequest(Request);
		Request->OnProcessRequestComplete().BindWeakLambda(Outer, [SuccessDelegate, ErrorDelegate](HTTP_LAMBDA_PARAMS)
		{
			CHECK_NETWORK
			FString Content = Response->GetContentAsString();
			TW_LOG(Verbose, TEXT("ThirdwebEngine::Marketplace::DirectListings::GetAll::Content=%s"), *Content)
			FString Error;
			if (TArray<TSharedPtr<FJsonValue>> JsonArray; ThirdwebUtils::Json::ParseEngineResponse(Content, JsonArray, Error))
			{
				EXECUTE_IF_BOUND(SuccessDelegate, FThirdwebMarketplaceDirectListing::FromJson(JsonArray))
			}
			else
			{
				EXECUTE_IF_BOUND(ErrorDelegate, Error)
			}
		});
		Request->ProcessRequest();
	}

	void Get(
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
			TW_LOG(Verbose, TEXT("ThirdwebEngine::Marketplace::DirectListings::Get::Content=%s"), *Content)
			FString Error;
			if (TSharedPtr<FJsonObject> JsonObject; ThirdwebUtils::Json::ParseEngineResponse(Content, JsonObject, Error))
			{
				EXECUTE_IF_BOUND(SuccessDelegate, FThirdwebMarketplaceDirectListing::FromJson(JsonObject))
			}
			else
			{
				EXECUTE_IF_BOUND(ErrorDelegate, Error)
			}
		});
		Request->ProcessRequest();
	}

	void IsBuyerApproved(
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
			TW_LOG(Verbose, TEXT("ThirdwebEngine::Marketplace::DirectListings::IsBuyerApproved::Content=%s"), *Content)
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

	void IsCurrencyApproved(
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
			TW_LOG(Verbose, TEXT("ThirdwebEngine::Marketplace::DirectListings::IsCurrencyApproved::Content=%s"), *Content)
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

	void Create(
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
			TW_LOG(Verbose, TEXT("ThirdwebEngine::Marketplace::DirectListings::Create::Content=%s"), *Content)
			HANDLE_QUEUE_ID_RESPONSE
		});
		Request->ProcessRequest();
	}

	void Update(
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
			TW_LOG(Verbose, TEXT("ThirdwebEngine::Marketplace::DirectListings::Update::Content=%s"), *Content)
			HANDLE_QUEUE_ID_RESPONSE
		});
		Request->ProcessRequest();
	}

	void Buy(
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
			TW_LOG(Verbose, TEXT("ThirdwebEngine::Marketplace::DirectListings::Buy::Content=%s"), *Content)
			HANDLE_QUEUE_ID_RESPONSE
		});
		Request->ProcessRequest();
	}

	void ApproveReservedBuyer(
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
			TW_LOG(Verbose, TEXT("ThirdwebEngine::Marketplace::DirectListings::ApproveReservedBuyer::Content=%s"), *Content)
			HANDLE_QUEUE_ID_RESPONSE
		});
		Request->ProcessRequest();
	}

	void RevokeReservedBuyerApproval(
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
			TW_LOG(Verbose, TEXT("ThirdwebEngine::Marketplace::DirectListings::RevokeReservedBuyerApproval::Content=%s"), *Content)
			HANDLE_QUEUE_ID_RESPONSE
		});
		Request->ProcessRequest();
	}

	void RevokeReservedCurrencyApproval(
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
			TW_LOG(Verbose, TEXT("ThirdwebEngine::Marketplace::DirectListings::RevokeReservedCurrencyApproval::Content=%s"), *Content)
			HANDLE_QUEUE_ID_RESPONSE
		});
		Request->ProcessRequest();
	}

	void Cancel(
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
			TW_LOG(Verbose, TEXT("ThirdwebEngine::Marketplace::DirectListings::Cancel::Content=%s"), *Content)
			HANDLE_QUEUE_ID_RESPONSE
		});
		Request->ProcessRequest();
	}
}
