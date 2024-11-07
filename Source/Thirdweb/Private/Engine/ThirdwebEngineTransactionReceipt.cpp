// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "Engine/ThirdwebEngineTransactionReceipt.h"

#include "ThirdwebUtils.h"

#include "Dom/JsonObject.h"

FThirdwebEngineTransactionReceiptLog FThirdwebEngineTransactionReceiptLog::FromJson(const TSharedPtr<FJsonObject>& JsonObject)
{
	FThirdwebEngineTransactionReceiptLog Log;
	if (JsonObject.IsValid())
	{
		if (ThirdwebUtils::Json::IsNotNullField<EJson::Number>(JsonObject, TEXT("transactionIndex")))
		{
			Log.TransactionIndex = JsonObject->GetNumberField(TEXT("transactionIndex"));
		}
		if (ThirdwebUtils::Json::IsNotNullField<EJson::Number>(JsonObject, TEXT("blockNumber")))
		{
			Log.BlockNumber = JsonObject->GetNumberField(TEXT("blockNumber"));
		}
		if (ThirdwebUtils::Json::IsNotNullField<EJson::String>(JsonObject, TEXT("transactionHash")))
		{
			Log.TransactionHash = JsonObject->GetStringField(TEXT("transactionHash"));
		}
		if (ThirdwebUtils::Json::IsNotNullField<EJson::String>(JsonObject, TEXT("address")))
		{
			Log.Address = JsonObject->GetStringField(TEXT("address"));
		}
		if (ThirdwebUtils::Json::IsNotNullField<EJson::Array>(JsonObject, TEXT("topics")))
		{
			JsonObject->TryGetStringArrayField(TEXT("topics"), Log.Topics);
		}
		if (ThirdwebUtils::Json::IsNotNullField<EJson::String>(JsonObject, TEXT("data")))
		{
			Log.Data = JsonObject->GetStringField(TEXT("data"));
		}
		if (ThirdwebUtils::Json::IsNotNullField<EJson::Number>(JsonObject, TEXT("logIndex")))
		{
			Log.LogIndex = JsonObject->GetNumberField(TEXT("logIndex"));
		}
		if (ThirdwebUtils::Json::IsNotNullField<EJson::String>(JsonObject, TEXT("blockHash")))
		{
			Log.BlockHash = JsonObject->GetStringField(TEXT("blockHash"));
		}
	}
	return Log;
}

TArray<FThirdwebEngineTransactionReceiptLog> FThirdwebEngineTransactionReceiptLog::FromJsonArray(const TArray<TSharedPtr<FJsonValue>>& JsonValueArray)
{
	TArray<FThirdwebEngineTransactionReceiptLog> Logs;
	for (int32 i = 0; i < JsonValueArray.Num(); i++)
	{
		if (JsonValueArray[i]->Type == EJson::Object)
		{
			Logs.Emplace(FromJson(JsonValueArray[i]->AsObject()));
		}
	}
	return Logs;
}

FThirdwebEngineTransactionReceipt FThirdwebEngineTransactionReceipt::FromJson(const TSharedPtr<FJsonObject>& JsonObject)
{
	FThirdwebEngineTransactionReceipt Receipt;
	if (JsonObject.IsValid())
	{
		if (ThirdwebUtils::Json::IsNotNullField<EJson::String>(JsonObject, TEXT("to")))
		{
			Receipt.To = JsonObject->GetStringField(TEXT("to"));
		}
		if (ThirdwebUtils::Json::IsNotNullField<EJson::String>(JsonObject, TEXT("from")))
		{
			Receipt.From = JsonObject->GetStringField(TEXT("from"));
		}
		if (ThirdwebUtils::Json::IsNotNullField<EJson::String>(JsonObject, TEXT("contractAddress")))
		{
			Receipt.ContractAddress = JsonObject->GetStringField(TEXT("contractAddress"));
		}
		if (ThirdwebUtils::Json::IsNotNullField<EJson::Number>(JsonObject, TEXT("transactionIndex")))
		{
			Receipt.TransactionIndex = JsonObject->GetNumberField(TEXT("transactionIndex"));
		}
		if (ThirdwebUtils::Json::IsNotNullField<EJson::String>(JsonObject, TEXT("gasUsed")))
		{
			Receipt.GasUsed = ThirdwebUtils::Internal::ParseInt64(JsonObject->GetStringField(TEXT("gasUsed")));
		}
		if (ThirdwebUtils::Json::IsNotNullField<EJson::String>(JsonObject, TEXT("logsBloom")))
		{
			Receipt.LogsBloom = JsonObject->GetStringField(TEXT("logsBloom"));
		}
		if (ThirdwebUtils::Json::IsNotNullField<EJson::String>(JsonObject, TEXT("blockHash")))
		{
			Receipt.BlockHash = JsonObject->GetStringField(TEXT("blockHash"));
		}
		if (ThirdwebUtils::Json::IsNotNullField<EJson::String>(JsonObject, TEXT("transactionHash")))
		{
			Receipt.TransactionHash = JsonObject->GetStringField(TEXT("transactionHash"));
		}
		if (ThirdwebUtils::Json::IsNotNullField<EJson::Array>(JsonObject, TEXT("logs")))
		{
			Receipt.Logs = FThirdwebEngineTransactionReceiptLog::FromJsonArray(JsonObject->GetArrayField(TEXT("logs")));
		}
		if (ThirdwebUtils::Json::IsNotNullField<EJson::Number>(JsonObject, TEXT("blockNumber")))
		{
			Receipt.BlockNumber = JsonObject->GetNumberField(TEXT("blockNumber"));
		}
		if (ThirdwebUtils::Json::IsNotNullField<EJson::Number>(JsonObject, TEXT("confirmations")))
		{
			Receipt.Confirmations = JsonObject->GetNumberField(TEXT("confirmations"));
		}
		if (ThirdwebUtils::Json::IsNotNullField<EJson::String>(JsonObject, TEXT("cumulativeGasUsed")))
		{
			Receipt.CumulativeGasUsed = ThirdwebUtils::Internal::ParseInt64(JsonObject->GetStringField(TEXT("cumulativeGasUsed")));
		}
		if (ThirdwebUtils::Json::IsNotNullField<EJson::String>(JsonObject, TEXT("effectiveGasPrice")))
		{
			Receipt.EffectiveGasPrice = ThirdwebUtils::Internal::ParseInt64(JsonObject->GetStringField(TEXT("effectiveGasPrice")));
		}
		if (ThirdwebUtils::Json::IsNotNullField<EJson::Number>(JsonObject, TEXT("status")))
		{
			Receipt.Status = JsonObject->GetNumberField(TEXT("status"));
		}
		if (ThirdwebUtils::Json::IsNotNullField<EJson::Number>(JsonObject, TEXT("type")))
		{
			Receipt.Type = JsonObject->GetNumberField(TEXT("type"));
		}
		if (ThirdwebUtils::Json::IsNotNullField<EJson::Boolean>(JsonObject, TEXT("byzantium")))
		{
			Receipt.Byzantium = JsonObject->GetBoolField(TEXT("byzantium"));
		}
	}
	return Receipt;
}
