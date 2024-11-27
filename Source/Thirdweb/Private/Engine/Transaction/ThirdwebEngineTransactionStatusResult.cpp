// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "Engine/Transaction/ThirdwebEngineTransactionStatusResult.h"

#include "ThirdwebUtils.h"
#include "Engine/ThirdwebEngineCommon.h"

FThirdwebEngineTransactionStatusResult::FThirdwebEngineTransactionStatusResult()
{
	ChainId = 0;
	Nonce = 0;
	GasLimit = 0;
	GasPrice = 0;
	MaxFeePerGas = 0;
	MaxPriorityFeePerGas = 0;
	TransactionType = 0;
	SentAtBlockNumber = 0;
	BlockNumber = 0;
	RetryCount = 0;
	RetryGasValues = false;
	RetryMaxFeePerGas = 0;
	RetryMaxPriorityFeePerGas = 0;
	CallGasLimit = 0;
	VerificationGasLimit = 0;
	PreVerificationGas = 0;
	OnChainTxStatus = 0;
	OnChainStatus = EThirdwebEngineTransactionOnChainStatus::Unknown;
	EffectiveGasPrice = 0;
	CumulativeGasUsed = 0;
	Status = EThirdwebEngineTransactionStatus::Unknown;
}

FThirdwebEngineTransactionStatusResult FThirdwebEngineTransactionStatusResult::FromJson(const TSharedPtr<FJsonObject>& JsonObject)
{
	FThirdwebEngineTransactionStatusResult Result;
	if (JsonObject.IsValid())
	{
		if (ThirdwebUtils::Json::IsNotNullField<EJson::String>(JsonObject, TEXT("queueId")))
		{
			Result.QueueId = JsonObject->GetStringField(TEXT("queueId"));
		}
		if (ThirdwebUtils::Json::IsNotNullField<EJson::String>(JsonObject,TEXT("status")))
		{
			Result.Status = ThirdwebUtils::ToTransactionStatus(JsonObject->GetStringField(TEXT("status")));
		}
		if (ThirdwebUtils::Json::IsNotNullField<EJson::String>(JsonObject,TEXT("chainId")))
		{
			Result.ChainId = ThirdwebUtils::Internal::ParseInt64(JsonObject->GetStringField(TEXT("chainId")));
		}
		if (ThirdwebUtils::Json::IsNotNullField<EJson::String>(JsonObject,TEXT("fromAddress")))
		{
			Result.FromAddress = JsonObject->GetStringField(TEXT("fromAddress"));
		}
		if (ThirdwebUtils::Json::IsNotNullField<EJson::String>(JsonObject,TEXT("toAddress")))
		{
			Result.ToAddress = JsonObject->GetStringField(TEXT("toAddress"));
		}
		if (ThirdwebUtils::Json::IsNotNullField<EJson::String>(JsonObject,TEXT("data")))
		{
			Result.Data = JsonObject->GetStringField(TEXT("data"));
		}
		if (ThirdwebUtils::Json::IsNotNullField<EJson::String>(JsonObject,TEXT("extension")))
		{
			Result.Extension = JsonObject->GetStringField(TEXT("extension"));
		}
		if (ThirdwebUtils::Json::IsNotNullField<EJson::String>(JsonObject,TEXT("value")))
		{
			Result.Value = JsonObject->GetStringField(TEXT("value"));
		}
		if (ThirdwebUtils::Json::IsNotNullField<EJson::Number>(JsonObject,TEXT("nonce")))
		{
			Result.Nonce = JsonObject->GetNumberField(TEXT("nonce"));
		}
		if (ThirdwebUtils::Json::IsNotNullField<EJson::String>(JsonObject,TEXT("gasLimit")))
		{
			Result.GasLimit = ThirdwebUtils::Internal::ParseInt64(JsonObject->GetStringField(TEXT("gasLimit")));
		}
		if (ThirdwebUtils::Json::IsNotNullField<EJson::String>(JsonObject,TEXT("gasPrice")))
		{
			Result.GasPrice = ThirdwebUtils::Internal::ParseInt64(JsonObject->GetStringField(TEXT("gasPrice")));
		}
		if (ThirdwebUtils::Json::IsNotNullField<EJson::String>(JsonObject,TEXT("maxFeePerGas")))
		{
			Result.MaxFeePerGas = ThirdwebUtils::Internal::ParseInt64(JsonObject->GetStringField(TEXT("maxFeePerGas")));
		}
		if (ThirdwebUtils::Json::IsNotNullField<EJson::String>(JsonObject,TEXT("maxPriorityFeePerGas")))
		{
			Result.MaxPriorityFeePerGas = ThirdwebUtils::Internal::ParseInt64(JsonObject->GetStringField(TEXT("maxPriorityFeePerGas")));
		}
		if (ThirdwebUtils::Json::IsNotNullField<EJson::String>(JsonObject,TEXT("transactionType")))
		{
			Result.TransactionType = ThirdwebUtils::Internal::ParseInt64(JsonObject->GetStringField(TEXT("transactionType")));
		}
		if (ThirdwebUtils::Json::IsNotNullField<EJson::String>(JsonObject,TEXT("transactionHash")))
		{
			Result.TransactionHash = JsonObject->GetStringField(TEXT("transactionHash"));
		}
		if (ThirdwebUtils::Json::IsNotNullField<EJson::String>(JsonObject,TEXT("queuedAt")))
		{
			 FDateTime::ParseIso8601(*JsonObject->GetStringField(TEXT("queuedAt")), Result.QueuedAt);
		}
		if (ThirdwebUtils::Json::IsNotNullField<EJson::String>(JsonObject,TEXT("sentAt")))
		{
			FDateTime::ParseIso8601(*JsonObject->GetStringField(TEXT("sentAt")), Result.SentAt);
		}
		if (ThirdwebUtils::Json::IsNotNullField<EJson::String>(JsonObject,TEXT("minedAt")))
		{
			FDateTime::ParseIso8601(*JsonObject->GetStringField(TEXT("minedAt")), Result.MinedAt);
		}
		if (ThirdwebUtils::Json::IsNotNullField<EJson::String>(JsonObject,TEXT("cancelledAt")))
		{
			FDateTime::ParseIso8601(*JsonObject->GetStringField(TEXT("cancelledAt")), Result.CancelledAt);
		}
		if (ThirdwebUtils::Json::IsNotNullField<EJson::String>(JsonObject,TEXT("deployedContractAddress")))
		{
			Result.DeployedContractAddress = JsonObject->GetStringField(TEXT("deployedContractAddress"));
		}
		if (ThirdwebUtils::Json::IsNotNullField<EJson::String>(JsonObject,TEXT("deployedContractType")))
		{
			Result.DeployedContractType = JsonObject->GetStringField(TEXT("deployedContractType"));
		}
		if (ThirdwebUtils::Json::IsNotNullField<EJson::String>(JsonObject,TEXT("errorMessage")))
		{
			Result.ErrorMessage = JsonObject->GetStringField(TEXT("errorMessage"));
		}
		if (ThirdwebUtils::Json::IsNotNullField<EJson::Number>(JsonObject,TEXT("sentAtBlockNumber")))
		{
			Result.SentAtBlockNumber = JsonObject->GetNumberField(TEXT("sentAtBlockNumber"));
		}
		if (ThirdwebUtils::Json::IsNotNullField<EJson::Number>(JsonObject,TEXT("blockNumber")))
		{
			Result.BlockNumber = JsonObject->GetNumberField(TEXT("blockNumber"));
		}
		if (ThirdwebUtils::Json::IsNotNullField<EJson::Number>(JsonObject,TEXT("retryCount")))
		{
			Result.RetryCount = JsonObject->GetNumberField(TEXT("retryCount"));
		}
		if (ThirdwebUtils::Json::IsNotNullField<EJson::Boolean>(JsonObject,TEXT("retryGasValues")))
		{
			Result.RetryGasValues = JsonObject->GetBoolField(TEXT("retryGasValues"));
		}
		if (ThirdwebUtils::Json::IsNotNullField<EJson::String>(JsonObject,TEXT("retryMaxFeePerGas")))
		{
			Result.RetryMaxFeePerGas = ThirdwebUtils::Internal::ParseInt64(JsonObject->GetStringField(TEXT("retryMaxFeePerGas")));
		}
		if (ThirdwebUtils::Json::IsNotNullField<EJson::String>(JsonObject,TEXT("retryMaxPriorityFeePerGas")))
		{
			Result.RetryMaxPriorityFeePerGas = ThirdwebUtils::Internal::ParseInt64(JsonObject->GetStringField(TEXT("retryMaxPriorityFeePerGas")));
		}
		if (ThirdwebUtils::Json::IsNotNullField<EJson::String>(JsonObject,TEXT("retryMaxPriorityFeePerGas")))
		{
			Result.RetryMaxPriorityFeePerGas = ThirdwebUtils::Internal::ParseInt64(JsonObject->GetStringField(TEXT("retryMaxPriorityFeePerGas")));
		}
		if (ThirdwebUtils::Json::IsNotNullField<EJson::String>(JsonObject,TEXT("signerAddress")))
		{
			Result.SignerAddress = JsonObject->GetStringField(TEXT("signerAddress"));
		}
		if (ThirdwebUtils::Json::IsNotNullField<EJson::String>(JsonObject,TEXT("accountAddress")))
		{
			Result.AccountAddress = JsonObject->GetStringField(TEXT("accountAddress"));
		}
		if (ThirdwebUtils::Json::IsNotNullField<EJson::String>(JsonObject,TEXT("target")))
		{
			Result.Target = JsonObject->GetStringField(TEXT("target"));
		}
		if (ThirdwebUtils::Json::IsNotNullField<EJson::String>(JsonObject,TEXT("sender")))
		{
			Result.Sender = JsonObject->GetStringField(TEXT("sender"));
		}
		if (ThirdwebUtils::Json::IsNotNullField<EJson::String>(JsonObject,TEXT("initCode")))
		{
			Result.InitCode = JsonObject->GetStringField(TEXT("initCode"));
		}
		if (ThirdwebUtils::Json::IsNotNullField<EJson::String>(JsonObject,TEXT("callData")))
		{
			Result.CallData = JsonObject->GetStringField(TEXT("callData"));
		}
		if (ThirdwebUtils::Json::IsNotNullField<EJson::String>(JsonObject,TEXT("callGasLimit")))
		{
			Result.CallGasLimit = ThirdwebUtils::Internal::ParseInt64(JsonObject->GetStringField(TEXT("callGasLimit")));
		}
		if (ThirdwebUtils::Json::IsNotNullField<EJson::String>(JsonObject,TEXT("verificationGasLimit")))
		{
			Result.VerificationGasLimit = ThirdwebUtils::Internal::ParseInt64(JsonObject->GetStringField(TEXT("verificationGasLimit")));
		}
		if (ThirdwebUtils::Json::IsNotNullField<EJson::String>(JsonObject,TEXT("preVerificationGas")))
		{
			Result.PreVerificationGas = ThirdwebUtils::Internal::ParseInt64(JsonObject->GetStringField(TEXT("preVerificationGas")));
		}
		if (ThirdwebUtils::Json::IsNotNullField<EJson::String>(JsonObject,TEXT("paymasterAndData")))
		{
			Result.PaymasterAndData = JsonObject->GetStringField(TEXT("paymasterAndData"));
		}
		if (ThirdwebUtils::Json::IsNotNullField<EJson::String>(JsonObject,TEXT("userOpHash")))
		{
			Result.UserOpHash = JsonObject->GetStringField(TEXT("userOpHash"));
		}
		if (ThirdwebUtils::Json::IsNotNullField<EJson::String>(JsonObject,TEXT("functionName")))
		{
			Result.FunctionName = JsonObject->GetStringField(TEXT("functionName"));
		}
		if (ThirdwebUtils::Json::IsNotNullField<EJson::String>(JsonObject,TEXT("functionArgs")))
		{
			TArray<TSharedPtr<FJsonValue>> JsonValues = ThirdwebUtils::Json::ToJsonArray(JsonObject->GetStringField(TEXT("functionArgs")));
			for (int i = 0; i < JsonValues.Num(); i++)
			{
				Result.FunctionArgs.Emplace(ThirdwebUtils::Json::ToString(JsonValues[i]));
			}
		}
		if (ThirdwebUtils::Json::IsNotNullField<EJson::Number>(JsonObject,TEXT("onChainTxStatus")))
		{
			Result.OnChainTxStatus = JsonObject->GetNumberField(TEXT("onChainTxStatus"));
		}
		if (ThirdwebUtils::Json::IsNotNullField<EJson::String>(JsonObject,TEXT("onchainStatus")))
		{
			Result.OnChainStatus = ThirdwebUtils::ToOnChainStatus(JsonObject->GetStringField(TEXT("onchainStatus")));
		}
		if (ThirdwebUtils::Json::IsNotNullField<EJson::String>(JsonObject,TEXT("effectiveGasPrice")))
		{
			Result.EffectiveGasPrice = ThirdwebUtils::Internal::ParseInt64(JsonObject->GetStringField(TEXT("effectiveGasPrice")));
		}
		if (ThirdwebUtils::Json::IsNotNullField<EJson::String>(JsonObject,TEXT("cumulativeGasUsed")))
		{
			Result.CumulativeGasUsed = ThirdwebUtils::Internal::ParseInt64(JsonObject->GetStringField(TEXT("cumulativeGasUsed")));
		}
	}
	return Result;
}
