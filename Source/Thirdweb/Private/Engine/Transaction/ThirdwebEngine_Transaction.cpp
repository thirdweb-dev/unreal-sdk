// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "Engine/Transaction/ThirdwebEngine_Transaction.h"

#include "ThirdwebLog.h"
#include "ThirdwebRuntimeSettings.h"
#include "ThirdwebUtils.h"
#include "Engine/ThirdwebEngine.h"
#include "Engine/Transaction/ThirdwebEngineTransactionReceipt.h"
#include "Engine/Transaction/ThirdwebEngineTransactionStatusResult.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Internal/ThirdwebURLSearchParams.h"

namespace ThirdwebEngine::Transaction
{
	FString FormatUrl(const FString& Endpoint, const FThirdwebURLSearchParams& Params)
	{
		return ThirdwebEngine::FormatUrl(TEXT("transaction"), Endpoint, Params);
	}

	FString FormatUrl(const int64 ChainId, const FString& Endpoint, const FThirdwebURLSearchParams& Params)
	{
		return FormatUrl(FString::Printf(TEXT("%lld/%s"), ChainId, *Endpoint), Params);
	}

	void GetStatus(const UObject* Outer, const FString& QueueId, const FGetTransactionStatusDelegate& SuccessDelegate, const FStringDelegate& ErrorDelegate)
		{
			const TSharedRef<IHttpRequest> Request = ThirdwebUtils::Internal::CreateEngineRequest();
			Request->SetURL(FormatUrl(FString::Printf(TEXT("status/%s"), *QueueId), {}));
			ThirdwebUtils::Internal::LogRequest(Request);
			Request->OnProcessRequestComplete().BindWeakLambda(Outer, [SuccessDelegate, ErrorDelegate](HTTP_LAMBDA_PARAMS)
			{
				CHECK_NETWORK
				FString Content = Response->GetContentAsString();
				TW_LOG(Verbose, TEXT("ThirdwebEngine::Transaction::GetStatus::Content=%s"), *Content)
				FString Error;
				if (TSharedPtr<FJsonObject> JsonObject; ThirdwebUtils::Json::ParseEngineResponse(Content, JsonObject, Error))
				{
					EXECUTE_IF_BOUND(SuccessDelegate, FThirdwebEngineTransactionStatusResult::FromJson(JsonObject))
				}
				else
				{
					EXECUTE_IF_BOUND(ErrorDelegate, Error)
				}
			});
			Request->ProcessRequest();
		}

		void GetReceipt(const UObject* Outer, const FString& TxHash, const int64 ChainId, const FGetTransactionReceiptDelegate& SuccessDelegate, const FStringDelegate& ErrorDelegate)
		{
			const TSharedRef<IHttpRequest> Request = ThirdwebUtils::Internal::CreateEngineRequest();
			Request->SetURL(FormatUrl(ChainId, FString::Printf(TEXT("tx-hash/%s"), *TxHash), {}));
			ThirdwebUtils::Internal::LogRequest(Request);
			Request->OnProcessRequestComplete().BindWeakLambda(Outer, [SuccessDelegate, ErrorDelegate](HTTP_LAMBDA_PARAMS)
			{
				CHECK_NETWORK
				FString Content = Response->GetContentAsString();
				TW_LOG(Verbose, TEXT("ThirdwebEngine::Transaction::GetReceipt::Content=%s"), *Content)
				FString Error;
				if (TSharedPtr<FJsonObject> JsonObject; ThirdwebUtils::Json::ParseEngineResponse(Content, JsonObject, Error))
				{
					EXECUTE_IF_BOUND(SuccessDelegate, FThirdwebEngineTransactionReceipt::FromJson(JsonObject))
				}
				else
				{
					EXECUTE_IF_BOUND(ErrorDelegate, Error)
				}
			});
			Request->ProcessRequest();
		}
}
