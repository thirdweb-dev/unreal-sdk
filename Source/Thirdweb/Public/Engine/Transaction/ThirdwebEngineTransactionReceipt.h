// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "ThirdwebEngineTransactionReceipt.generated.h"

class FJsonObject;
class FJsonValue;

USTRUCT(BlueprintType, DisplayName="Transaction Receipt Log")
struct THIRDWEB_API FThirdwebEngineTransactionReceiptLog
{
	GENERATED_BODY()

	/** A contract or wallet address */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Thirdweb|Engine")
	int32 TransactionIndex = 0;

	/** A contract or wallet address */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Thirdweb|Engine")
	int64 BlockNumber = 0;

	/** A contract or wallet address */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Thirdweb|Engine")
	FString TransactionHash;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Thirdweb|Engine")
	FString Address;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Thirdweb|Engine")
	TArray<FString> Topics;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Thirdweb|Engine")
	FString Data;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Thirdweb|Engine")
	int32 LogIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Thirdweb|Engine")
	FString BlockHash;

	static FThirdwebEngineTransactionReceiptLog FromJson(const TSharedPtr<FJsonObject>& JsonObject);
	static TArray<FThirdwebEngineTransactionReceiptLog> FromJsonArray(const TArray<TSharedPtr<FJsonValue>>& JsonValueArray);
};

USTRUCT(BlueprintType, DisplayName="Transaction Receipt")
struct THIRDWEB_API FThirdwebEngineTransactionReceipt
{
	GENERATED_BODY()
	
	/** A contract or wallet address */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Thirdweb|Engine")
	FString To;

	/** A contract or wallet address */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Thirdweb|Engine")
	FString From;

	/** A contract or wallet address */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Thirdweb|Engine")
	FString ContractAddress;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Thirdweb|Engine")
	int32 TransactionIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Thirdweb|Engine")
	int64 GasUsed = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Thirdweb|Engine")
	FString LogsBloom;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Thirdweb|Engine")
	FString BlockHash;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Thirdweb|Engine")
	FString TransactionHash;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Thirdweb|Engine")
	TArray<FThirdwebEngineTransactionReceiptLog> Logs;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Thirdweb|Engine")
	int64 BlockNumber = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Thirdweb|Engine")
	int64 Confirmations = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Thirdweb|Engine")
	int64 CumulativeGasUsed = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Thirdweb|Engine")
	int64 EffectiveGasPrice = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Thirdweb|Engine")
	int32 Status = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Thirdweb|Engine")
	int32 Type = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Thirdweb|Engine")
	bool Byzantium = false;

	static FThirdwebEngineTransactionReceipt FromJson(const TSharedPtr<FJsonObject>& JsonObject);
};
