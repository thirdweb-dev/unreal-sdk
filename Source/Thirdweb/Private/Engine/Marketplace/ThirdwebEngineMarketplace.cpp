// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "Engine/Marketplace/ThirdwebEngineMarketplace.h"

#include "ThirdwebLog.h"
#include "ThirdwebRuntimeSettings.h"
#include "ThirdwebUtils.h"
#include "Containers/AnsiString.h"
#include "Engine/ThirdwebEngine.h"
#include "Engine/Marketplace/DirectListings/ThirdwebMarketplaceListing.h"
#include "Engine/Transaction/ThirdwebEngineTransactionOverrides.h"
#include "Engine/Transaction/ThirdwebEngineTransactionReceipt.h"
#include "Engine/Transaction/ThirdwebEngineTransactionStatusResult.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Internal/ThirdwebURLSearchParams.h"
#include "Kismet/KismetStringLibrary.h"
#include "Wallets/ThirdwebSmartWalletHandle.h"

#define CHECK_NETWORK \
	if (!bConnectedSuccessfully) { \
		EXECUTE_IF_BOUND(ErrorDelegate, TEXT("Network Connection Failed")) \
		return; \
	}

#define HTTP_LAMBDA_PARAMS FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully

namespace ThirdwebEngine::Marketplace
{
	FString FormatUrl(const int64 ChainId, const FString& ContractAddress, const FString& Section, const TCHAR* InFormatString, const FStringFormatOrderedArguments& InOrderedArguments)
	{
		return UThirdwebRuntimeSettings::GetEngineBaseUrl() + FString::Printf(TEXT("/marketplace/%lld/%s/%s"), ChainId, *ContractAddress, *Section) + FString::Format(
			InFormatString, InOrderedArguments);
	}

	namespace DirectListings
	{
		FString FormatUrl(const int64 ChainId, const FString& ContractAddress, const TCHAR* InFormatString)
		{
			return FormatUrl(ChainId, ContractAddress, InFormatString, {});
		}

		FString FormatUrl(const int64 ChainId, const FString& ContractAddress, const TCHAR* InFormatString, const FStringFormatOrderedArguments& InOrderedArguments)
		{
			return Marketplace::FormatUrl(ChainId, ContractAddress, TEXT("direct-listings"), InFormatString, InOrderedArguments);
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

			Request->SetURL(FormatUrl(Chain, ContractAddress, TEXT("/get-all")) + Params.ToString(true));

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

			Request->SetURL(FormatUrl(Chain, ContractAddress, TEXT("/get-all-valid")) + Params.ToString(true));

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

			Request->SetURL(FormatUrl(Chain, ContractAddress, TEXT("/get-listing")) + Params.ToString(true));

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

			Request->SetURL(FormatUrl(Chain, ContractAddress, TEXT("/is-buyer-approved-for-listing")) + Params.ToString(true));

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

			Request->SetURL(FormatUrl(Chain, ContractAddress, TEXT("/is-currency-approved-for-listing")) + Params.ToString(true));

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

			Request->SetURL(FormatUrl(Chain, ContractAddress, TEXT("/get-total-count")));

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
			const bool bSimulateTx,
			const int64 Chain,
			const FString& ContractAddress,
			const FString& BackendWalletAddress,
			const FString& IdempotencyKey,
			const FString& AccountAddress,
			const FString& AccountFactoryAddress,
			const FString& AccountSalt,
			const FThirdwebMarketplaceCreateListingRequest& Request,
			const FStringDelegate& SuccessDelegate,
			const FStringDelegate& ErrorDelegate
		)
		{
		}

		void UpdateListing(
			const UObject* Outer,
			const bool bSimulateTx,
			const int64 Chain,
			const FString& ContractAddress,
			const FString& BackendWalletAddress,
			const FString& IdempotencyKey,
			const FString& AccountAddress,
			const FString& AccountFactoryAddress,
			const FString& AccountSalt,
			const FString& ListingId,
			const FStringDelegate& SuccessDelegate,
			const FStringDelegate& ErrorDelegate
		)
		{
		}

		void BuyFromListing(
			const UObject* Outer,
			const bool bSimulateTx,
			const int64 Chain,
			const FString& ContractAddress,
			const FString& BackendWalletAddress,
			const FString& IdempotencyKey,
			const FString& AccountAddress,
			const FString& AccountFactoryAddress,
			const FString& AccountSalt,
			const FString& ListingId,
			const FString& Quantity,
			const FString& Buyer,
			const FStringDelegate& SuccessDelegate,
			const FStringDelegate& ErrorDelegate
		)
		{
		}

		void ApproveBuyerForReservedListing(
			const UObject* Outer,
			const bool bSimulateTx,
			const int64 Chain,
			const FString& ContractAddress,
			const FString& BackendWalletAddress,
			const FString& IdempotencyKey,
			const FString& AccountAddress,
			const FString& AccountFactoryAddress,
			const FString& AccountSalt,
			const FString& ListingId,
			const FString& Buyer,
			const FStringDelegate& SuccessDelegate,
			const FStringDelegate& ErrorDelegate
		)
		{
		}

		void RevokeBuyerApprovalForReservedListing(
			const UObject* Outer,
			const bool bSimulateTx,
			const int64 Chain,
			const FString& ContractAddress,
			const FString& BackendWalletAddress,
			const FString& IdempotencyKey,
			const FString& AccountAddress,
			const FString& AccountFactoryAddress,
			const FString& AccountSalt,
			const FString& ListingId,
			const FString& BuyerAddress,
			const FStringDelegate& SuccessDelegate,
			const FStringDelegate& ErrorDelegate
		)
		{
		}

		void RevokeCurrencyApprovalForListing(
			const UObject* Outer,
			const bool bSimulateTx,
			const int64 Chain,
			const FString& ContractAddress,
			const FString& BackendWalletAddress,
			const FString& IdempotencyKey,
			const FString& AccountAddress,
			const FString& AccountFactoryAddress,
			const FString& AccountSalt,
			const FString& ListingId,
			const FString& CurrencyContractAddress,
			const FStringDelegate& SuccessDelegate,
			const FStringDelegate& ErrorDelegate
		)
		{
		}

		void CancelListing(
			const UObject* Outer,
			const bool bSimulateTx,
			const int64 Chain,
			const FString& ContractAddress,
			const FString& BackendWalletAddress,
			const FString& IdempotencyKey,
			const FString& AccountAddress,
			const FString& AccountFactoryAddress,
			const FString& AccountSalt,
			const FString& ListingId,
			const FStringDelegate& SuccessDelegate,
			const FStringDelegate& ErrorDelegate
		)
		{
		}
	}

	namespace EnglishAuctions
	{
		FString FormatUrl(const int64 ChainId, const FString& ContractAddress, const TCHAR* InFormatString)
		{
			return Marketplace::FormatUrl(ChainId, ContractAddress, TEXT("english-auctions"), InFormatString, {});
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

			Request->SetURL(FormatUrl(Chain, ContractAddress, TEXT("/get-all")) + Params.ToString(true));

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

			Request->SetURL(FormatUrl(Chain, ContractAddress, TEXT("/get-all-valid")) + Params.ToString(true));

			ThirdwebUtils::Internal::LogRequest(Request);
			Request->OnProcessRequestComplete().BindWeakLambda(Outer, [SuccessDelegate, ErrorDelegate](HTTP_LAMBDA_PARAMS)
			{
				CHECK_NETWORK
				FString Content = Response->GetContentAsString();
				TW_LOG(Verbose, TEXT("ThirdwebEngine::Marketplace::EnglishAuctions::GetAllValid::Content=%s"), *Content)
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

		void GetAuction(
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

			Request->SetURL(FormatUrl(Chain, ContractAddress, TEXT("/get-auction")) + Params.ToString(true));

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

			Request->SetURL(FormatUrl(Chain, ContractAddress, TEXT("/get-bid-buffer-bps")) + Params.ToString(true));

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
			const FGetCurrencyValueDelegate& SuccessDelegate,
			const FStringDelegate& ErrorDelegate
		)
		{
			const TSharedRef<IHttpRequest> Request = ThirdwebUtils::Internal::CreateEngineRequest();

			FThirdwebURLSearchParams Params;
			Params.Set(TEXT("listingId"), ListingId, ListingId.IsNumeric() && !ListingId.StartsWith("-"));

			Request->SetURL(FormatUrl(Chain, ContractAddress, TEXT("/get-minimum-next-bid")) + Params.ToString(true));

			ThirdwebUtils::Internal::LogRequest(Request);
			Request->OnProcessRequestComplete().BindWeakLambda(Outer, [SuccessDelegate, ErrorDelegate](HTTP_LAMBDA_PARAMS)
			{
				CHECK_NETWORK
				FString Content = Response->GetContentAsString();
				TW_LOG(Verbose, TEXT("ThirdwebEngine::Marketplace::EnglishAuctions::GetMinimumNextBid::Content=%s"), *Content)
				FString Error;
				if (TSharedPtr<FJsonObject> JsonObject; ThirdwebUtils::Json::ParseEngineResponse(Content, JsonObject, Error))
				{
					EXECUTE_IF_BOUND(SuccessDelegate, FThirdwebAssetCurrencyValue::FromJson(JsonObject))
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

			Request->SetURL(FormatUrl(Chain, ContractAddress, TEXT("/get-winning-bid")) + Params.ToString(true));

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

			Request->SetURL(FormatUrl(Chain, ContractAddress, TEXT("/get-total-count")));

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

			Request->SetURL(FormatUrl(Chain, ContractAddress, TEXT("/is-winning-bid")) + Params.ToString(true));

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

			Request->SetURL(FormatUrl(Chain, ContractAddress, TEXT("/get-winner")) + Params.ToString(true));

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

		void BuyoutAuction(
			const UObject* Outer,
			const bool bSimulateTx,
			const FString& ListingId,
			const int64 Chain,
			const FString& ContractAddress,
			const FStringDelegate& SuccessDelegate,
			const FStringDelegate& ErrorDelegate
		)
		{
		}

		void CancelAuction(
			const UObject* Outer,
			const bool bSimulateTx,
			const FString& ListingId,
			const int64 Chain,
			const FString& ContractAddress,
			const FStringDelegate& SuccessDelegate,
			const FStringDelegate& ErrorDelegate
		)
		{
		}

		void CreateAuction(
			const UObject* Outer,
			const bool bSimulateTx,
			const FThirdwebMarketplaceCreateAuctionRequest& Request,
			const int64 Chain,
			const FString& ContractAddress,
			const FStringDelegate& SuccessDelegate,
			const FStringDelegate& ErrorDelegate
		)
		{
		}

		void CloseAuctionForBidder(
			const UObject* Outer,
			const bool bSimulateTx,
			const FString& ListingId,
			const int64 Chain,
			const FString& ContractAddress,
			const FStringDelegate& SuccessDelegate,
			const FStringDelegate& ErrorDelegate
		)
		{
		}

		void CloseAuctionForSeller(
			const UObject* Outer,
			const bool bSimulateTx,
			const FString& ListingId,
			const int64 Chain,
			const FString& ContractAddress,
			const FStringDelegate& SuccessDelegate,
			const FStringDelegate& ErrorDelegate
		)
		{
		}

		void ExecuteSale(
			const UObject* Outer,
			const bool bSimulateTx,
			const FString& ListingId,
			const int64 Chain,
			const FString& ContractAddress,
			const FStringDelegate& SuccessDelegate,
			const FStringDelegate& ErrorDelegate
		)
		{
		}

		void MakeBid(
			const UObject* Outer,
			const bool bSimulateTx,
			const FString& ListingId,
			const FString& BidAmount,
			const int64 Chain,
			const FString& ContractAddress,
			const FStringDelegate& SuccessDelegate,
			const FStringDelegate& ErrorDelegate
		)
		{
		}
	}

	namespace Offers
	{
		FString FormatUrl(const int64 ChainId, const FString& ContractAddress, const TCHAR* InFormatString)
		{
			return Marketplace::FormatUrl(ChainId, ContractAddress, TEXT("offers"), InFormatString, {});
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

			Request->SetURL(FormatUrl(Chain, ContractAddress, TEXT("/get-all")) + Params.ToString(true));

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

		void GetAllValid(
			const UObject* Outer,
			const int32 Count,
			const FString& Offeror,
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
			Params.Set(TEXT("offeror"), Offeror, !Offeror.IsEmpty());
			Params.Set(TEXT("start"), Start, Start >= 0);
			Params.Set(TEXT("tokenContract"), TokenContract, !TokenContract.IsEmpty());
			Params.Set(TEXT("tokenId"), TokenContract, TokenId.IsNumeric() && !TokenId.StartsWith("-"));

			Request->SetURL(FormatUrl(Chain, ContractAddress, TEXT("/get-all-valid")) + Params.ToString(true));

			ThirdwebUtils::Internal::LogRequest(Request);
			Request->OnProcessRequestComplete().BindWeakLambda(Outer, [SuccessDelegate, ErrorDelegate](HTTP_LAMBDA_PARAMS)
			{
				CHECK_NETWORK
				FString Content = Response->GetContentAsString();
				TW_LOG(Verbose, TEXT("ThirdwebEngine::Marketplace::Offers::GetAllValid::Content=%s"), *Content)
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

		void GetOffer(const UObject* Outer, const FString& OfferId, const int64 Chain, const FString& ContractAddress, const FGetOfferDelegate& SuccessDelegate, const FStringDelegate& ErrorDelegate)
		{
			const TSharedRef<IHttpRequest> Request = ThirdwebUtils::Internal::CreateEngineRequest();

			FThirdwebURLSearchParams Params;
			Params.Set(TEXT("offerId"), OfferId, OfferId.IsNumeric() && !OfferId.StartsWith("-"));

			Request->SetURL(FormatUrl(Chain, ContractAddress, TEXT("/get-offer")) + Params.ToString(true));

			ThirdwebUtils::Internal::LogRequest(Request);
			Request->OnProcessRequestComplete().BindWeakLambda(Outer, [SuccessDelegate, ErrorDelegate](HTTP_LAMBDA_PARAMS)
			{
				CHECK_NETWORK
				FString Content = Response->GetContentAsString();
				TW_LOG(Verbose, TEXT("ThirdwebEngine::Marketplace::Offers::GetOffer::Content=%s"), *Content)
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

			Request->SetURL(FormatUrl(Chain, ContractAddress, TEXT("/get-total-count")));

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

		void MakeOffer(
			const UObject* Outer,
			const bool bSimulateTx,
			const int64 Chain,
			const FString& ContractAddress,
			const FString& BackendWalletAddress,
			const FString& IdempotencyKey,
			const FString& AccountAddress,
			const FString& AccountFactoryAddress,
			const FString& AccountSalt,
			const FThirdwebMarketplaceMakeOfferRequest& Request,
			const FStringDelegate& SuccessDelegate,
			const FStringDelegate& ErrorDelegate
		)
		{
		}

		void CancelOffer(
			const UObject* Outer,
			const bool bSimulateTx,
			const int64 Chain,
			const FString& ContractAddress,
			const FString& BackendWalletAddress,
			const FString& IdempotencyKey,
			const FString& AccountAddress,
			const FString& AccountFactoryAddress,
			const FString& AccountSalt,
			const FString& OfferId,
			const FStringDelegate& SuccessDelegate,
			const FStringDelegate& ErrorDelegate
		)
		{
		}

		void AcceptOffer(
			const UObject* Outer,
			const bool bSimulateTx,
			const int64 Chain,
			const FString& ContractAddress,
			const FString& BackendWalletAddress,
			const FString& IdempotencyKey,
			const FString& AccountAddress,
			const FString& AccountFactoryAddress,
			const FString& AccountSalt,
			const FString& OfferId,
			const FStringDelegate& SuccessDelegate,
			const FStringDelegate& ErrorDelegate
		)
		{
		}
	}
}
