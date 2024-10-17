// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "AsyncTasks/AsyncTaskThirdwebBase.h"

#include "Interfaces/IHttpRequest.h"

#include "Wallets/ThirdwebInAppWalletHandle.h"
#include "Wallets/ThirdwebSmartWalletHandle.h"

#include "AsyncTaskThirdwebEngineWriteContract.generated.h"


class FJsonObject;

USTRUCT(BlueprintType)
struct THIRDWEB_API FThirdwebTransactionOverrides
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Thirdweb|Engine")
	int64 Gas = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Thirdweb|Engine")
	int64 MaxFeePerGas = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Thirdweb|Engine")
	int64 MaxPriorityFeePerGas = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Thirdweb|Engine")
	int64 Value = 0;

	bool IsDefault() const { return Gas == 0 && MaxFeePerGas == 0 && MaxPriorityFeePerGas == 0 && Value == 0; }
	TSharedPtr<FJsonObject> ToJson() const;
};

UCLASS(Blueprintable, BlueprintType)
class THIRDWEB_API UAsyncTaskThirdwebEngineWriteContract : public UAsyncTaskThirdwebBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject", AdvancedDisplay="IdempotencyKey,FactoryAddress,TxOverrides,Abi,bSimulateTx", AutoCreateRefTerm="Args,SmartWallet,TxOverrides"), Category="Thirdweb|Engine")
	static UAsyncTaskThirdwebEngineWriteContract* WriteContract(UObject* WorldContextObject,
	                                                            const int64 ChainId,
	                                                            const FString& ContractAddress,
	                                                            const FString& BackendWalletAddress,
	                                                            const FSmartWalletHandle& SmartWallet,
	                                                            const FString& FactoryAddress,
	                                                            const FString& IdempotencyKey,
	                                                            const FString& FunctionName,
	                                                            const TArray<FString>& Args,
	                                                            const FThirdwebTransactionOverrides& TxOverrides,
	                                                            const FString& Abi,
	                                                            const bool bSimulateTx);

	virtual void Activate() override;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FWriteContractDelegate, const FString&, QueueId, const FString&, Error);

	UPROPERTY(BlueprintAssignable)
	FWriteContractDelegate Success;

	UPROPERTY(BlueprintAssignable)
	FWriteContractDelegate Failed;

protected:
	UPROPERTY(Transient)
	int64 ChainId;

	UPROPERTY(Transient)
	FString ContractAddress;

	UPROPERTY(Transient)
	FString BackendWalletAddress;

	UPROPERTY(Transient)
	FSmartWalletHandle SmartWallet;

	UPROPERTY(Transient)
	FString FactoryAddress;

	UPROPERTY(Transient)
	FString IdempotencyKey;

	UPROPERTY(Transient)
	FString FunctionName;

	UPROPERTY(Transient)
	TArray<FString> Args;

	UPROPERTY(Transient)
	FThirdwebTransactionOverrides TxOverrides;

	UPROPERTY(Transient)
	FString Abi;

	UPROPERTY(Transient)
	bool bSimulateTx;

private:
	virtual void HandleResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
	void HandleFailed(const FString& Error);
};
