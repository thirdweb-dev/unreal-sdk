// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "Engine/Marketplace/EnglishAuctions/ThirdwebEngine_Marketplace_EnglishAuctions.h"

#include "ThirdwebLog.h"
#include "ThirdwebUtils.h"
#include "Engine/Marketplace/ThirdwebEngine_Marketplace.h"
#include "Engine/Marketplace/DirectListings/ThirdwebMarketplaceListing.h"
#include "Engine/Marketplace/EnglishAuctions/ThirdwebMarketplaceCreateEnglishAuctionRequest.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Internal/ThirdwebHeaders.h"
#include "Internal/ThirdwebURLSearchParams.h"

namespace ThirdwebEngine::Marketplace::EnglishAuctions
{
	FString FormatUrl(const int64 ChainId, const FString& ContractAddress, const FString& Endpoint, const FThirdwebURLSearchParams& Params)
	{
		return Marketplace::FormatUrl(ChainId, ContractAddress, TEXT("english-auctions"), Endpoint, Params);
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

		Request->SetURL(FormatUrl(Chain, ContractAddress,  bOnlyValid ? TEXT("/get-all-valid") : TEXT("/get-all"), Params));

		ThirdwebUtils::Internal::LogRequest(Request);
		Request->OnProcessRequestComplete().BindWeakLambda(Outer, [SuccessDelegate, ErrorDelegate](HTTP_LAMBDA_PARAMS)
		{
			CHECK_NETWORK
			FString Content = Response->GetContentAsString();
			TW_LOG(Verbose, TEXT("ThirdwebEngine::Marketplace::EnglishAuctions::GetAll::Content=%s"), *Content)
			FString Error;
			if (TArray<TSharedPtr<FJsonValue>> JsonArray; ThirdwebUtils::Json::ParseEngineResponse(Content, JsonArray, Error))
			{
				EXECUTE_IF_BOUND(SuccessDelegate, FThirdwebMarketplaceEnglishAuction::FromJson(JsonArray))
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
		const FGetAuctionDelegate& SuccessDelegate,
		const FStringDelegate& ErrorDelegate
	)
	{
		const TSharedRef<IHttpRequest> Request = ThirdwebUtils::Internal::CreateEngineRequest();

		FThirdwebURLSearchParams Params;
		Params.Set(TEXT("listingId"), ListingId, ListingId.IsNumeric() && !ListingId.StartsWith("-"));

		Request->SetURL(FormatUrl(Chain, ContractAddress, TEXT("/get-auction"), Params));

		ThirdwebUtils::Internal::LogRequest(Request);
		Request->OnProcessRequestComplete().BindWeakLambda(Outer, [SuccessDelegate, ErrorDelegate](HTTP_LAMBDA_PARAMS)
		{
			CHECK_NETWORK
			FString Content = Response->GetContentAsString();
			TW_LOG(Verbose, TEXT("ThirdwebEngine::Marketplace::EnglishAuctions::GetAuction::Content=%s"), *Content)
			FString Error;
			if (TSharedPtr<FJsonObject> JsonObject; ThirdwebUtils::Json::ParseEngineResponse(Content, JsonObject, Error))
			{
				EXECUTE_IF_BOUND(SuccessDelegate, FThirdwebMarketplaceEnglishAuction::FromJson(JsonObject))
			}
			else
			{
				EXECUTE_IF_BOUND(ErrorDelegate, Error)
			}
		});
		Request->ProcessRequest();
	}

	void GetBidBufferBps(
		const UObject* Outer,
		const FString& ListingId,
		const int64 Chain,
		const FString& ContractAddress,
		const FInt32Delegate& SuccessDelegate,
		const FStringDelegate& ErrorDelegate
	)
	{
		const TSharedRef<IHttpRequest> Request = ThirdwebUtils::Internal::CreateEngineRequest();

		FThirdwebURLSearchParams Params;
		Params.Set(TEXT("listingId"), ListingId, ListingId.IsNumeric() && !ListingId.StartsWith("-"));

		Request->SetURL(FormatUrl(Chain, ContractAddress, TEXT("/get-bid-buffer-bps"), Params));

		ThirdwebUtils::Internal::LogRequest(Request);
		Request->OnProcessRequestComplete().BindWeakLambda(Outer, [SuccessDelegate, ErrorDelegate](HTTP_LAMBDA_PARAMS)
		{
			CHECK_NETWORK
			FString Content = Response->GetContentAsString();
			TW_LOG(Verbose, TEXT("ThirdwebEngine::Marketplace::EnglishAuctions::GetBidBufferBps::Content=%s"), *Content)
			FString Error;
			if (TSharedPtr<FJsonValue> JsonValue; ThirdwebUtils::Json::ParseEngineResponse(Content, JsonValue, Error))
			{
				EXECUTE_IF_BOUND(SuccessDelegate, JsonValue->AsNumber())
			}
			else
			{
				EXECUTE_IF_BOUND(ErrorDelegate, Error)
			}
		});
		Request->ProcessRequest();
	}

	void GetMinimumNextBid(
		const UObject* Outer,
		const FString& ListingId,
		const int64 Chain,
		const FString& ContractAddress,
		const FStringDelegate& SuccessDelegate,
		const FStringDelegate& ErrorDelegate
	)
	{
		const TSharedRef<IHttpRequest> Request = ThirdwebUtils::Internal::CreateEngineRequest();

		FThirdwebURLSearchParams Params;
		Params.Set(TEXT("listingId"), ListingId, ListingId.IsNumeric() && !ListingId.StartsWith("-"));

		Request->SetURL(FormatUrl(Chain, ContractAddress, TEXT("/get-minimum-next-bid"), Params));

		ThirdwebUtils::Internal::LogRequest(Request);
		Request->OnProcessRequestComplete().BindWeakLambda(Outer, [SuccessDelegate, ErrorDelegate](HTTP_LAMBDA_PARAMS)
		{
			CHECK_NETWORK
			FString Content = Response->GetContentAsString();
			TW_LOG(Verbose, TEXT("ThirdwebEngine::Marketplace::EnglishAuctions::GetMinimumNextBid::Content=%s"), *Content)
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

	void GetWinningBid(
		const UObject* Outer,
		const FString& ListingId,
		const int64 Chain,
		const FString& ContractAddress,
		const FGetBidDelegate& SuccessDelegate,
		const FStringDelegate& ErrorDelegate
	)
	{
		const TSharedRef<IHttpRequest> Request = ThirdwebUtils::Internal::CreateEngineRequest();

		FThirdwebURLSearchParams Params;
		Params.Set(TEXT("listingId"), ListingId, ListingId.IsNumeric() && !ListingId.StartsWith("-"));

		Request->SetURL(FormatUrl(Chain, ContractAddress, TEXT("/get-winning-bid"), Params));

		ThirdwebUtils::Internal::LogRequest(Request);
		Request->OnProcessRequestComplete().BindWeakLambda(Outer, [SuccessDelegate, ErrorDelegate](HTTP_LAMBDA_PARAMS)
		{
			CHECK_NETWORK
			FString Content = Response->GetContentAsString();
			TW_LOG(Verbose, TEXT("ThirdwebEngine::Marketplace::EnglishAuctions::GetWinningBid::Content=%s"), *Content)
			FString Error;
			if (TSharedPtr<FJsonObject> JsonObject; ThirdwebUtils::Json::ParseEngineResponse(Content, JsonObject, Error))
			{
				EXECUTE_IF_BOUND(SuccessDelegate, FThirdwebMarketplaceBid::FromJson(JsonObject))
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

		Request->SetURL(FormatUrl(Chain, ContractAddress, TEXT("/get-total-count"), {}));

		ThirdwebUtils::Internal::LogRequest(Request);
		Request->OnProcessRequestComplete().BindWeakLambda(Outer, [SuccessDelegate, ErrorDelegate](HTTP_LAMBDA_PARAMS)
		{
			CHECK_NETWORK
			FString Content = Response->GetContentAsString();
			TW_LOG(Verbose, TEXT("ThirdwebEngine::Marketplace::EnglishAuctions::GetTotalCount::Content=%s"), *Content)
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

	void IsWinningBid(
		const UObject* Outer,
		const FString& ListingId,
		const FString& BidAmount,
		const int64 Chain,
		const FString& ContractAddress,
		const FBoolDelegate& SuccessDelegate,
		const FStringDelegate& ErrorDelegate
	)
	{
		const TSharedRef<IHttpRequest> Request = ThirdwebUtils::Internal::CreateEngineRequest();

		FThirdwebURLSearchParams Params;
		Params.Set(TEXT("listingId"), ListingId, ListingId.IsNumeric() && !ListingId.StartsWith("-"));
		Params.Set(TEXT("bidAmount"), BidAmount, !BidAmount.IsEmpty());

		Request->SetURL(FormatUrl(Chain, ContractAddress, TEXT("/is-winning-bid"), Params));

		ThirdwebUtils::Internal::LogRequest(Request);
		Request->OnProcessRequestComplete().BindWeakLambda(Outer, [SuccessDelegate, ErrorDelegate](HTTP_LAMBDA_PARAMS)
		{
			CHECK_NETWORK
			FString Content = Response->GetContentAsString();
			TW_LOG(Verbose, TEXT("ThirdwebEngine::Marketplace::EnglishAuctions::IsWinningBid::Content=%s"), *Content)
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

	void GetWinner(
		const UObject* Outer,
		const FString& ListingId,
		const int64 Chain,
		const FString& ContractAddress,
		const FStringDelegate& SuccessDelegate,
		const FStringDelegate& ErrorDelegate
	)
	{
		const TSharedRef<IHttpRequest> Request = ThirdwebUtils::Internal::CreateEngineRequest();

		FThirdwebURLSearchParams Params;
		Params.Set(TEXT("listingId"), ListingId, ListingId.IsNumeric() && !ListingId.StartsWith("-"));

		Request->SetURL(FormatUrl(Chain, ContractAddress, TEXT("/get-winner"), Params));

		ThirdwebUtils::Internal::LogRequest(Request);
		Request->OnProcessRequestComplete().BindWeakLambda(Outer, [SuccessDelegate, ErrorDelegate](HTTP_LAMBDA_PARAMS)
		{
			CHECK_NETWORK
			FString Content = Response->GetContentAsString();
			TW_LOG(Verbose, TEXT("ThirdwebEngine::Marketplace::EnglishAuctions::GetWinner::Content=%s"), *Content)
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

	void Buyout(
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

		Request->SetURL(FormatUrl(Chain, ContractAddress, TEXT("buyout-auction"), Params));

		MakePostHeaders(BackendWalletAddress, IdempotencyKey, Account).UpdateRequest(Request);

		TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
		JsonObject->SetStringField(TEXT("listingId"), ListingId);
		Request->SetContentAsString(ThirdwebUtils::Json::ToString(JsonObject));

		ThirdwebUtils::Internal::LogRequest(Request);
		Request->OnProcessRequestComplete().BindWeakLambda(Outer, [SuccessDelegate, ErrorDelegate](HTTP_LAMBDA_PARAMS)
		{
			CHECK_NETWORK
			FString Content = Response->GetContentAsString();
			TW_LOG(Verbose, TEXT("ThirdwebEngine::Marketplace::EnglishAuctions::BuyoutAuction::Content=%s"), *Content)
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

		Request->SetURL(FormatUrl(Chain, ContractAddress, TEXT("cancel-auction"), Params));

		MakePostHeaders(BackendWalletAddress, IdempotencyKey, Account).UpdateRequest(Request);

		TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
		JsonObject->SetStringField(TEXT("listingId"), ListingId);
		Request->SetContentAsString(ThirdwebUtils::Json::ToString(JsonObject));

		ThirdwebUtils::Internal::LogRequest(Request);
		Request->OnProcessRequestComplete().BindWeakLambda(Outer, [SuccessDelegate, ErrorDelegate](HTTP_LAMBDA_PARAMS)
		{
			CHECK_NETWORK
			FString Content = Response->GetContentAsString();
			TW_LOG(Verbose, TEXT("ThirdwebEngine::Marketplace::EnglishAuctions::Cancel::Content=%s"), *Content)
			HANDLE_QUEUE_ID_RESPONSE
		});
		Request->ProcessRequest();
	}

	void Create(
		const UObject* Outer,
		const int64 Chain,
		const FString& ContractAddress,
		const FString& BackendWalletAddress,
		const FThirdwebAccountIdentifierParams& Account,
		const FThirdwebMarketplaceCreateEnglishAuctionRequest& EnglishAuctionRequest,
		const FString& IdempotencyKey,
		const bool bSimulateTx,
		const FStringDelegate& SuccessDelegate,
		const FStringDelegate& ErrorDelegate
	)
	{
		const TSharedRef<IHttpRequest> Request = ThirdwebUtils::Internal::CreateEngineRequest(TEXT("POST"));
		FThirdwebURLSearchParams Params;
		Params.Set(TEXT("simulateTx"), true, bSimulateTx);

		Request->SetURL(FormatUrl(Chain, ContractAddress, TEXT("cancel-auction"), Params));

		MakePostHeaders(BackendWalletAddress, IdempotencyKey, Account).UpdateRequest(Request);
		
		Request->SetContentAsString(ThirdwebUtils::Json::ToString(EnglishAuctionRequest.ToJson()));

		ThirdwebUtils::Internal::LogRequest(Request);
		Request->OnProcessRequestComplete().BindWeakLambda(Outer, [SuccessDelegate, ErrorDelegate](HTTP_LAMBDA_PARAMS)
		{
			CHECK_NETWORK
			FString Content = Response->GetContentAsString();
			TW_LOG(Verbose, TEXT("ThirdwebEngine::Marketplace::EnglishAuctions::Cancel::Content=%s"), *Content)
			HANDLE_QUEUE_ID_RESPONSE
		});
		Request->ProcessRequest();
	}

	void CloseForBidder(
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

		Request->SetURL(FormatUrl(Chain, ContractAddress, TEXT("close-auction-for-bidder"), Params));

		MakePostHeaders(BackendWalletAddress, IdempotencyKey, Account).UpdateRequest(Request);

		TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
		JsonObject->SetStringField(TEXT("listingId"), ListingId);
		Request->SetContentAsString(ThirdwebUtils::Json::ToString(JsonObject));

		ThirdwebUtils::Internal::LogRequest(Request);
		Request->OnProcessRequestComplete().BindWeakLambda(Outer, [SuccessDelegate, ErrorDelegate](HTTP_LAMBDA_PARAMS)
		{
			CHECK_NETWORK
			FString Content = Response->GetContentAsString();
			TW_LOG(Verbose, TEXT("ThirdwebEngine::Marketplace::EnglishAuctions::CloseForBidder::Content=%s"), *Content)
			HANDLE_QUEUE_ID_RESPONSE
		});
		Request->ProcessRequest();
	}

	void CloseForSeller(
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

		Request->SetURL(FormatUrl(Chain, ContractAddress, TEXT("close-auction-for-seller"), Params));

		MakePostHeaders(BackendWalletAddress, IdempotencyKey, Account).UpdateRequest(Request);

		TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
		JsonObject->SetStringField(TEXT("listingId"), ListingId);
		Request->SetContentAsString(ThirdwebUtils::Json::ToString(JsonObject));

		ThirdwebUtils::Internal::LogRequest(Request);
		Request->OnProcessRequestComplete().BindWeakLambda(Outer, [SuccessDelegate, ErrorDelegate](HTTP_LAMBDA_PARAMS)
		{
			CHECK_NETWORK
			FString Content = Response->GetContentAsString();
			TW_LOG(Verbose, TEXT("ThirdwebEngine::Marketplace::EnglishAuctions::CloseForSeller::Content=%s"), *Content)
			HANDLE_QUEUE_ID_RESPONSE
		});
		Request->ProcessRequest();
	}

	void ExecuteSale(
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

		Request->SetURL(FormatUrl(Chain, ContractAddress, TEXT("execute-sale"), Params));

		MakePostHeaders(BackendWalletAddress, IdempotencyKey, Account).UpdateRequest(Request);

		TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
		JsonObject->SetStringField(TEXT("listingId"), ListingId);
		Request->SetContentAsString(ThirdwebUtils::Json::ToString(JsonObject));

		ThirdwebUtils::Internal::LogRequest(Request);
		Request->OnProcessRequestComplete().BindWeakLambda(Outer, [SuccessDelegate, ErrorDelegate](HTTP_LAMBDA_PARAMS)
		{
			CHECK_NETWORK
			FString Content = Response->GetContentAsString();
			TW_LOG(Verbose, TEXT("ThirdwebEngine::Marketplace::EnglishAuctions::ExecuteSale::Content=%s"), *Content)
			HANDLE_QUEUE_ID_RESPONSE
		});
		Request->ProcessRequest();
	}

	void Bid(
		const UObject* Outer,
		const int64 Chain,
		const FString& ContractAddress,
		const FString& BackendWalletAddress,
		const FThirdwebAccountIdentifierParams& Account,
		const FString& ListingId,
		const FString& BidAmount,
		const FString& IdempotencyKey,
		const bool bSimulateTx,
		const FStringDelegate& SuccessDelegate,
		const FStringDelegate& ErrorDelegate
	)
	{
		const TSharedRef<IHttpRequest> Request = ThirdwebUtils::Internal::CreateEngineRequest(TEXT("POST"));
		FThirdwebURLSearchParams Params;
		Params.Set(TEXT("simulateTx"), true, bSimulateTx);

		Request->SetURL(FormatUrl(Chain, ContractAddress, TEXT("make-bid"), Params));

		MakePostHeaders(BackendWalletAddress, IdempotencyKey, Account).UpdateRequest(Request);

		TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
		JsonObject->SetStringField(TEXT("listingId"), ListingId);
		JsonObject->SetStringField(TEXT("bidAmount"), BidAmount);
		Request->SetContentAsString(ThirdwebUtils::Json::ToString(JsonObject));

		ThirdwebUtils::Internal::LogRequest(Request);
		Request->OnProcessRequestComplete().BindWeakLambda(Outer, [SuccessDelegate, ErrorDelegate](HTTP_LAMBDA_PARAMS)
		{
			CHECK_NETWORK
			FString Content = Response->GetContentAsString();
			TW_LOG(Verbose, TEXT("ThirdwebEngine::Marketplace::EnglishAuctions::Bid::Content=%s"), *Content)
			HANDLE_QUEUE_ID_RESPONSE
		});
		Request->ProcessRequest();
	}
}
