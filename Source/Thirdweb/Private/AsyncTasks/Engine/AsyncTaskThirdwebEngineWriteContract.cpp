// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Engine/AsyncTaskThirdwebEngineWriteContract.h"

#include "HttpModule.h"
#include "ThirdwebLog.h"
#include "ThirdwebRuntimeSettings.h"
#include "ThirdwebUtils.h"

#include "Dom/JsonObject.h"

#include "Interfaces/IHttpResponse.h"

#include "Kismet/KismetStringLibrary.h"

UAsyncTaskThirdwebEngineWriteContract* UAsyncTaskThirdwebEngineWriteContract::WriteContract(
	UObject* WorldContextObject,
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
	const bool bSimulateTx)
{
	if (!WorldContextObject)
	{
		return nullptr;
	}
	NEW_TASK
	Task->ChainId = ChainId;
	Task->ContractAddress = ContractAddress.TrimStartAndEnd();
	Task->BackendWalletAddress = BackendWalletAddress.TrimStartAndEnd();
	Task->SmartWallet = SmartWallet;
	Task->FactoryAddress = FactoryAddress.TrimStartAndEnd();
	Task->IdempotencyKey = IdempotencyKey.TrimStartAndEnd();
	Task->FunctionName = FunctionName.TrimStartAndEnd();
	Task->Args = Args;
	Task->TxOverrides = TxOverrides;
	Task->Abi = Abi.TrimStartAndEnd();
	Task->bSimulateTx = bSimulateTx;
	Task->RegisterWithGameInstance(WorldContextObject);
	return Task;
}

void UAsyncTaskThirdwebEngineWriteContract::Activate()
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
	FString Content = ThirdwebUtils::Json::ToString(JsonObject);
	Request->SetContentAsString(Content);
	Request->SetURL(
		FString::Format(
			TEXT("{0}/contract/{1}/{2}/write{3}"),
			{
				UThirdwebRuntimeSettings::GetEngineBaseUrl(),
				FString::Printf(TEXT("%lld"), ChainId),
				ContractAddress,
				bSimulateTx ? TEXT("?simulateTx=true") : TEXT("")
			}
		)
	);
	ThirdwebUtils::Internal::LogRequest(Request, {UThirdwebRuntimeSettings::GetEngineAccessToken()});
	Request->OnProcessRequestComplete().BindUObject(this, &ThisClass::HandleResponse);
	Request->ProcessRequest();
}

void UAsyncTaskThirdwebEngineWriteContract::HandleResponse(FHttpRequestPtr, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	if (bConnectedSuccessfully)
	{
		const FString Content = Response->GetContentAsString();
		TW_LOG(Verbose, TEXT("UAsyncTaskThirdwebEngineWriteContract::HandleResponse::Content=%s"), *Content)
		FString Error;
		if (TSharedPtr<FJsonObject> JsonObject; ThirdwebUtils::Json::ParseEngineResponse(Content, JsonObject, Error))
		{
			FString QueueId = TEXT("Unknown");
			if (JsonObject->HasTypedField<EJson::String>(TEXT("queueId")))
			{
				QueueId = JsonObject->GetStringField(TEXT("queueId"));
			}
			Success.Broadcast(QueueId, TEXT(""));
			SetReadyToDestroy();
		}
		else return HandleFailed(Error);
	}
	else return HandleFailed(TEXT("Network connection failed"));
}

void UAsyncTaskThirdwebEngineWriteContract::HandleFailed(const FString& Error)
{
	Failed.Broadcast(TEXT(""), Error);
	SetReadyToDestroy();
}
