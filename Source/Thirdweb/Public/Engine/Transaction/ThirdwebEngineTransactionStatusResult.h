// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "Engine/ThirdwebEngineCommon.h"
#include "Misc/DateTime.h"
#include "ThirdwebEngineTransactionStatusResult.generated.h"

enum class EThirdwebEngineTransactionStatus : uint8;
class FJsonObject;

USTRUCT(BlueprintType, DisplayName="Transaction Status Result")
struct THIRDWEB_API FThirdwebEngineTransactionStatusResult
{
	GENERATED_BODY()

	FThirdwebEngineTransactionStatusResult();

	/** An identifier for an enqueued blockchain write call */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, DisplayName="Queue ID", Category="Thirdweb|Engine")
	FString QueueId;

	/** The current state of the transaction. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Thirdweb|Engine")
	EThirdwebEngineTransactionStatus Status;

	/** The chain ID for the transaction */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, DisplayName="Chain ID", Category="Thirdweb|Engine")
	int64 ChainId;

	/** The backend wallet submitting the transaction */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Thirdweb|Engine")
	FString FromAddress;

	/** The contract address to be called */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Thirdweb|Engine")
	FString ToAddress;

	/** Encoded calldata */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Thirdweb|Engine")
	FString Data;

	/** The extension detected by thirdweb */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Thirdweb|Engine")
	FString Extension;

	/** The amount of native currency to send */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Thirdweb|Engine")
	FString Value;
	
	/** The nonce used by the backend wallet for this transaction */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Thirdweb|Engine")
	int64 Nonce;

	/** The max gas unit limit */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Thirdweb|Engine")
	int64 GasLimit;
	
	/** The gas price used */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Thirdweb|Engine")
	int64 GasPrice;

	/** The max fee per gas (EIP-1559) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Thirdweb|Engine")
	int64 MaxFeePerGas;

	/** The max priority fee per gas (EIP-1559) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Thirdweb|Engine")
	int64 MaxPriorityFeePerGas;

	/** The type of transaction */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Thirdweb|Engine")
	int32 TransactionType;

	/** A transaction hash */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Thirdweb|Engine")
	FString TransactionHash;

	/** When the transaction is enqueued */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Thirdweb|Engine")
	FDateTime QueuedAt;

	/** When the transaction is submitted to mempool */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Thirdweb|Engine")
	FDateTime SentAt;

	/** When the transaction is mined on chain */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Thirdweb|Engine")
	FDateTime MinedAt;

	/** When the transaction is cancelled */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Thirdweb|Engine")
	FDateTime CancelledAt;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Thirdweb|Engine")
	FString DeployedContractAddress;
	
	/** The type of deployed contract */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Thirdweb|Engine")
	FString DeployedContractType;

	/** The error that occurred */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Thirdweb|Engine")
	FString ErrorMessage;

	/** The block number when the transaction is submitted to mempool */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Thirdweb|Engine")
	int64 SentAtBlockNumber;

	/** The block number when the transaction is mined */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Thirdweb|Engine")
	int64 BlockNumber;

	/** The number of retry attempts */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Thirdweb|Engine")
	int32 RetryCount;

	/** Whether to replace gas values on the next retry */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Thirdweb|Engine")
	bool RetryGasValues;

	/** The max fee per gas to use on retry */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Thirdweb|Engine")
	int64 RetryMaxFeePerGas;

	/** The max priority fee per gas to use on retry */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Thirdweb|Engine")
	int64 RetryMaxPriorityFeePerGas;

	/** The max priority fee per gas to use on retry */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Thirdweb|Engine")
	FString SignerAddress;
	
	/** A contract or wallet address */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Thirdweb|Engine")
	FString AccountAddress;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Thirdweb|Engine")
	FString AccountSalt;

	/** A contract or wallet address */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Thirdweb|Engine")
	FString AccountFactoryAddress;

	/** A contract or wallet address */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Thirdweb|Engine")
	FString Target;

	/** A contract or wallet address */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Thirdweb|Engine")
	FString Sender;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Thirdweb|Engine")
	FString InitCode;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Thirdweb|Engine")
	FString CallData;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Thirdweb|Engine")
	int64 CallGasLimit;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Thirdweb|Engine")
	int64 VerificationGasLimit;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Thirdweb|Engine")
	int64 PreVerificationGas;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Thirdweb|Engine")
	FString PaymasterAndData;

	/** A transaction hash */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Thirdweb|Engine")
	FString UserOpHash;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Thirdweb|Engine")
	FString FunctionName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Thirdweb|Engine")
	TArray<FString> FunctionArgs;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Thirdweb|Engine")
	int32 OnChainTxStatus;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Thirdweb|Engine")
	EThirdwebEngineTransactionOnChainStatus OnChainStatus;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Thirdweb|Engine")
	int64 EffectiveGasPrice;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Thirdweb|Engine")
	int64 CumulativeGasUsed;

	static FThirdwebEngineTransactionStatusResult FromJson(const TSharedPtr<FJsonObject>& JsonObject);
};
