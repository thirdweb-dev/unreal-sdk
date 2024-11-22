// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "Engine/ThirdwebEngine.h"

#include "ThirdwebLog.h"
#include "ThirdwebRuntimeSettings.h"
#include "ThirdwebUtils.h"
#include "Containers/AnsiString.h"
#include "Engine/Marketplace/ThirdwebEngineMarketplace.h"
#include "Engine/Transaction/ThirdwebEngineTransactionOverrides.h"
#include "Engine/Transaction/ThirdwebEngineTransactionReceipt.h"
#include "Engine/Transaction/ThirdwebEngineTransactionStatusResult.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Kismet/KismetStringLibrary.h"
#include "Wallets/ThirdwebSmartWalletHandle.h"

#define CHECK_NETWORK \
	if (!bConnectedSuccessfully) { \
		EXECUTE_IF_BOUND(ErrorDelegate, TEXT("Network Connection Failed")) \
		return; \
	}

#define HTTP_LAMBDA_PARAMS FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully

namespace ThirdwebEngine
{
	namespace Contract
	{
		void Read(
			const UObject* Outer,
			const int64 ChainId,
			const FString& ContractAddress,
			const FString& FunctionName,
			const TArray<FString>& Args,
			const FStringDelegate& SuccessDelegate,
			const FStringDelegate& ErrorDelegate
		)
		{
			const TSharedRef<IHttpRequest> Request = ThirdwebUtils::Internal::CreateEngineRequest();
			Request->SetURL(
				Utils::FormatUrl(
					TEXT("/contract/{0}/{1}/read?functionName={2}{3}"),
					{
						FString::Printf(TEXT("%lld"), ChainId),
						ContractAddress,
						FunctionName,
						Args.Num() > 0 ? TEXT("&args=") + UKismetStringLibrary::JoinStringArray(Args, TEXT(",")) : TEXT("")
					}
				)
			);
			ThirdwebUtils::Internal::LogRequest(Request);
			Request->OnProcessRequestComplete().BindWeakLambda(Outer, [SuccessDelegate, ErrorDelegate](HTTP_LAMBDA_PARAMS)
			{
				CHECK_NETWORK
				FString Content = Response->GetContentAsString();
				TW_LOG(Verbose, TEXT("ThirdwebEngine::Contract::Read::Content=%s"), *Content)
				FString Error;
				if (TSharedPtr<FJsonObject> JsonObject; ThirdwebUtils::Json::ParseEngineResponse(Content, JsonObject, Error))
				{
					EXECUTE_IF_BOUND(SuccessDelegate, Content)
				}
				else
				{
					EXECUTE_IF_BOUND(ErrorDelegate, Error)
				}
			});
			Request->ProcessRequest();
		}

		void Write(
			const UObject* Outer,
			const int64 ChainId,
			const FString& ContractAddress,
			const FString& BackendWalletAddress,
			const FSmartWalletHandle& SmartWallet,
			const FString& FactoryAddress,
			const FString& IdempotencyKey,
			const FString& FunctionName,
			const TArray<FString>& Args,
			const FThirdwebEngineTransactionOverrides& TxOverrides,
			const FString& Abi,
			const bool bSimulateTx,
			const FStringDelegate& SuccessDelegate,
			const FStringDelegate& ErrorDelegate
		)
		{
			const TSharedRef<IHttpRequest> Request = ThirdwebUtils::Internal::CreateEngineRequest(TEXT("POST"));
			Request->SetHeader(TEXT("x-backend-wallet-address"), BackendWalletAddress);
			if (!IdempotencyKey.IsEmpty())
			{
				Request->SetHeader(TEXT("x-idempotency-key"), IdempotencyKey);
			}
			if (SmartWallet.IsValid())
			{
				Request->SetHeader(TEXT("x-account-address"), SmartWallet.ToAddress());
			}
			if (!FactoryAddress.IsEmpty())
			{
				Request->SetHeader(TEXT("x-account-factory-address"), FactoryAddress);
			}
			{
				TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
				JsonObject->SetStringField(TEXT("functionName"), FunctionName);
				TArray<TSharedPtr<FJsonValue>> JsonArgs;
				for (const FString& Arg : Args)
				{
					JsonArgs.Emplace(MakeShareable(new FJsonValueString(Arg)));
				}
				JsonObject->SetArrayField(TEXT("args"), JsonArgs);
				if (!TxOverrides.IsDefault())
				{
					JsonObject->SetObjectField(TEXT("txOverrides"), TxOverrides.ToJson());
				}
				if (!Abi.IsEmpty())
				{
					JsonObject->SetArrayField(TEXT("abi"), ThirdwebUtils::Json::ToJsonArray(Abi));
				}
				Request->SetContentAsString(ThirdwebUtils::Json::ToString(JsonObject));
			}
			Request->SetURL(
				Utils::FormatUrl(
					TEXT("/contract/{0}/{1}/write{2}"),
					{
						FString::Printf(TEXT("%lld"), ChainId),
						ContractAddress,
						bSimulateTx ? TEXT("?simulateTx=true") : TEXT("")
					}
				)
			);
			ThirdwebUtils::Internal::LogRequest(Request);
			Request->OnProcessRequestComplete().BindWeakLambda(Outer, [SuccessDelegate, ErrorDelegate](HTTP_LAMBDA_PARAMS)
			{
				CHECK_NETWORK
				FString Content = Response->GetContentAsString();
				TW_LOG(Verbose, TEXT("ThirdwebEngine::Contract::Write::Content=%s"), *Content)
				FString Error;
				if (TSharedPtr<FJsonObject> JsonObject; ThirdwebUtils::Json::ParseEngineResponse(Content, JsonObject, Error))
				{
					FString QueueId = TEXT("Unknown");
					if (JsonObject->HasTypedField<EJson::String>(TEXT("queueId")))
					{
						QueueId = JsonObject->GetStringField(TEXT("queueId"));
					}
					EXECUTE_IF_BOUND(SuccessDelegate, QueueId)
				}
				else
				{
					EXECUTE_IF_BOUND(ErrorDelegate, Error)
				}
			});
			Request->ProcessRequest();
		}
	}

	namespace Transaction
	{
		void GetStatus(const UObject* Outer, const FString& QueueId, const FGetTransactionStatusDelegate& SuccessDelegate, const FStringDelegate& ErrorDelegate)
		{
			const TSharedRef<IHttpRequest> Request = ThirdwebUtils::Internal::CreateEngineRequest();
			Request->SetURL(Utils::FormatUrl(TEXT("/transaction/status/{0}"), {QueueId}));
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
			Request->SetURL(Utils::FormatUrl(TEXT("/transaction/{0}/tx-hash/{1}"), {FString::Printf(TEXT("%lld"), ChainId), TxHash}));
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
	
	namespace Utils
	{
		FString FormatUrl(const TCHAR* InFormatString, const FStringFormatOrderedArguments& InOrderedArguments)
		{
			return UThirdwebRuntimeSettings::GetEngineBaseUrl() + FString::Format(InFormatString, InOrderedArguments);
		}
	}
}
