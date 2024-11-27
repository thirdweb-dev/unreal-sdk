// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "Engine/Marketplace/Offers/ThirdwebEngine_Marketplace_Offers.h"

#include "ThirdwebLog.h"
#include "ThirdwebUtils.h"
#include "Engine/ThirdwebAccountIdentifierParams.h"
#include "Engine/Marketplace/ThirdwebEngine_Marketplace.h"
#include "Engine/Marketplace/DirectListings/ThirdwebMarketplaceDirectListing.h"
#include "Engine/Marketplace/Offers/ThirdwebMarketplaceMakeOfferRequest.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Internal/ThirdwebHeaders.h"
#include "Internal/ThirdwebURLSearchParams.h"

namespace ThirdwebEngine::Marketplace::Offers
{
	FString FormatUrl(const int64 ChainId, const FString& ContractAddress, const FString& Endpoint, const FThirdwebURLSearchParams& Params)
	{
		return Marketplace::FormatUrl(ChainId, ContractAddress, TEXT("offers"), Endpoint, Params);
	}

	void GetAll(
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
	)
	{
		const TSharedRef<IHttpRequest> Request = ThirdwebUtils::Internal::CreateEngineRequest();

		FThirdwebURLSearchParams Params;
		Params.Set(TEXT("count"), Count, Count > 0);
		Params.Set(TEXT("offeror"), Offeror, !Offeror.IsEmpty());
		Params.Set(TEXT("start"), Start, Start >= 0);
		Params.Set(TEXT("tokenContract"), TokenContract, !TokenContract.IsEmpty());
		Params.Set(TEXT("tokenId"), TokenContract, TokenId.IsNumeric() && !TokenId.StartsWith("-"));

		Request->SetURL(FormatUrl(Chain, ContractAddress, bOnlyValid ? TEXT("/get-all-valid") : TEXT("/get-all"), Params));

		ThirdwebUtils::Internal::LogRequest(Request);
		Request->OnProcessRequestComplete().BindWeakLambda(Outer, [SuccessDelegate, ErrorDelegate](HTTP_LAMBDA_PARAMS)
		{
			CHECK_NETWORK
			FString Content = Response->GetContentAsString();
			TW_LOG(Verbose, TEXT("ThirdwebEngine::Marketplace::Offers::GetAll::Content=%s"), *Content)
			FString Error;
			if (TArray<TSharedPtr<FJsonValue>> JsonArray; ThirdwebUtils::Json::ParseEngineResponse(Content, JsonArray, Error))
			{
				EXECUTE_IF_BOUND(SuccessDelegate, FThirdwebMarketplaceOffer::FromJson(JsonArray))
			}
			else
			{
				EXECUTE_IF_BOUND(ErrorDelegate, Error)
			}
		});
		Request->ProcessRequest();
	}

	void Get(const UObject* Outer, const FString& OfferId, const int64 Chain, const FString& ContractAddress, const FGetOfferDelegate& SuccessDelegate, const FStringDelegate& ErrorDelegate)
	{
		const TSharedRef<IHttpRequest> Request = ThirdwebUtils::Internal::CreateEngineRequest();

		FThirdwebURLSearchParams Params;
		Params.Set(TEXT("offerId"), OfferId, OfferId.IsNumeric() && !OfferId.StartsWith("-"));

		Request->SetURL(FormatUrl(Chain, ContractAddress, TEXT("/get-offer"), Params));

		ThirdwebUtils::Internal::LogRequest(Request);
		Request->OnProcessRequestComplete().BindWeakLambda(Outer, [SuccessDelegate, ErrorDelegate](HTTP_LAMBDA_PARAMS)
		{
			CHECK_NETWORK
			FString Content = Response->GetContentAsString();
			TW_LOG(Verbose, TEXT("ThirdwebEngine::Marketplace::Offers::Get::Content=%s"), *Content)
			FString Error;
			if (TSharedPtr<FJsonObject> JsonObject; ThirdwebUtils::Json::ParseEngineResponse(Content, JsonObject, Error))
			{
				EXECUTE_IF_BOUND(SuccessDelegate, FThirdwebMarketplaceOffer::FromJson(JsonObject))
			}
			else
			{
				EXECUTE_IF_BOUND(ErrorDelegate, Error)
			}
		});
		Request->ProcessRequest();
	}

	void GetTotalCount(
		const UObject* Outer,
		const int64 Chain,
		const FString& ContractAddress,
		const FStringDelegate& SuccessDelegate,
		const FStringDelegate& ErrorDelegate
	)
	{
		const TSharedRef<IHttpRequest> Request = ThirdwebUtils::Internal::CreateEngineRequest();

		Request->SetURL(FormatUrl(Chain, ContractAddress, TEXT("/get-total-count"), {}));

		ThirdwebUtils::Internal::LogRequest(Request);
		Request->OnProcessRequestComplete().BindWeakLambda(Outer, [SuccessDelegate, ErrorDelegate](HTTP_LAMBDA_PARAMS)
		{
			CHECK_NETWORK
			FString Content = Response->GetContentAsString();
			TW_LOG(Verbose, TEXT("ThirdwebEngine::Marketplace::Offers::GetTotalCount::Content=%s"), *Content)
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

	void Offer(
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
	)
	{
		const TSharedRef<IHttpRequest> Request = ThirdwebUtils::Internal::CreateEngineRequest(TEXT("POST"));
		FThirdwebURLSearchParams Params;
		Params.Set(TEXT("simulateTx"), true, bSimulateTx);

		Request->SetURL(FormatUrl(Chain, ContractAddress, TEXT("make-offer"), Params));

		MakePostHeaders(BackendWalletAddress, IdempotencyKey, Account).UpdateRequest(Request);

		Request->SetContentAsString(ThirdwebUtils::Json::ToString(OfferRequest.ToJson()));

		ThirdwebUtils::Internal::LogRequest(Request);
		Request->OnProcessRequestComplete().BindWeakLambda(Outer, [SuccessDelegate, ErrorDelegate](HTTP_LAMBDA_PARAMS)
		{
			CHECK_NETWORK
			FString Content = Response->GetContentAsString();
			TW_LOG(Verbose, TEXT("ThirdwebEngine::Marketplace::Offers::Offer::Content=%s"), *Content)
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
		const FString& OfferId,
		const FString& IdempotencyKey,
		const bool bSimulateTx,
		const FStringDelegate& SuccessDelegate,
		const FStringDelegate& ErrorDelegate
	)
	{
		const TSharedRef<IHttpRequest> Request = ThirdwebUtils::Internal::CreateEngineRequest(TEXT("POST"));
		FThirdwebURLSearchParams Params;
		Params.Set(TEXT("simulateTx"), true, bSimulateTx);

		Request->SetURL(FormatUrl(Chain, ContractAddress, TEXT("cancel-offer"), Params));

		MakePostHeaders(BackendWalletAddress, IdempotencyKey, Account).UpdateRequest(Request);

		TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
		JsonObject->SetStringField(TEXT("offerId"), OfferId);
		Request->SetContentAsString(ThirdwebUtils::Json::ToString(JsonObject));

		ThirdwebUtils::Internal::LogRequest(Request);
		Request->OnProcessRequestComplete().BindWeakLambda(Outer, [SuccessDelegate, ErrorDelegate](HTTP_LAMBDA_PARAMS)
		{
			CHECK_NETWORK
			FString Content = Response->GetContentAsString();
			TW_LOG(Verbose, TEXT("ThirdwebEngine::Marketplace::Offers::Cancel::Content=%s"), *Content)
			HANDLE_QUEUE_ID_RESPONSE
		});
		Request->ProcessRequest();
	}

	void Accept(
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
	)
	{
		const TSharedRef<IHttpRequest> Request = ThirdwebUtils::Internal::CreateEngineRequest(TEXT("POST"));
		FThirdwebURLSearchParams Params;
		Params.Set(TEXT("simulateTx"), true, bSimulateTx);

		Request->SetURL(FormatUrl(Chain, ContractAddress, TEXT("accept-offer"), Params));

		MakePostHeaders(BackendWalletAddress, IdempotencyKey, Account).UpdateRequest(Request);
		TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
		JsonObject->SetStringField(TEXT("offerId"), OfferId);
		Request->SetContentAsString(ThirdwebUtils::Json::ToString(JsonObject));

		ThirdwebUtils::Internal::LogRequest(Request);
		Request->OnProcessRequestComplete().BindWeakLambda(Outer, [SuccessDelegate, ErrorDelegate](HTTP_LAMBDA_PARAMS)
		{
			CHECK_NETWORK
			FString Content = Response->GetContentAsString();
			TW_LOG(Verbose, TEXT("ThirdwebEngine::Marketplace::Offers::Accept::Content=%s"), *Content)
			HANDLE_QUEUE_ID_RESPONSE
		});
		Request->ProcessRequest();
	}
}
