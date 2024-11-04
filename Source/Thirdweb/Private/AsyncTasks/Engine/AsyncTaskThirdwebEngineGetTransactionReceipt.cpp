// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Engine/AsyncTaskThirdwebEngineGetTransactionReceipt.h"

#include "HttpModule.h"
#include "ThirdwebLog.h"
#include "ThirdwebRuntimeSettings.h"
#include "ThirdwebUtils.h"

#include "Dom/JsonObject.h"

#include "Engine/ThirdwebEngineTransactionReceipt.h"

#include "Interfaces/IHttpResponse.h"

#include "Kismet/KismetStringLibrary.h"

#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

UAsyncTaskThirdwebEngineGetTransactionReceipt* UAsyncTaskThirdwebEngineGetTransactionReceipt::GetTransactionReceipt(UObject* WorldContextObject, const FString& TxHash, const int64 ChainID)
{
	if (!WorldContextObject)
	{
		return nullptr;
	}
	NEW_TASK
	Task->TransactionHash = TxHash;
	Task->ChainId = ChainID;
	RR_TASK
}

void UAsyncTaskThirdwebEngineGetTransactionReceipt::Activate()
{
	const TSharedRef<IHttpRequest> Request = ThirdwebUtils::Internal::CreateEngineRequest();
	Request->SetURL(FString::Printf(TEXT("%s/transaction/%lld/tx-hash/%s"), *UThirdwebRuntimeSettings::GetEngineBaseUrl(), ChainId, *TransactionHash));
	ThirdwebUtils::Internal::LogRequest(Request, {UThirdwebRuntimeSettings::GetEngineAccessToken()});
	Request->OnProcessRequestComplete().BindUObject(this, &ThisClass::HandleResponse);
	Request->ProcessRequest();
}

void UAsyncTaskThirdwebEngineGetTransactionReceipt::HandleResponse(FHttpRequestPtr, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	if (bConnectedSuccessfully)
	{
		FString Content = Response->GetContentAsString();
		TW_LOG(Verbose, TEXT("UAsyncTaskThirdwebEngineGetTransactionReceipt::HandleResponse::Content=%s"), *Content)
		FString Error;
		if (TSharedPtr<FJsonObject> JsonObject; ThirdwebUtils::Json::ParseEngineResponse(Content, JsonObject, Error))
		{
			Success.Broadcast(FThirdwebEngineTransactionReceipt::FromJson(JsonObject), TEXT(""));
			SetReadyToDestroy();
		}
		else return HandleFailed(Error);
	}
	else return HandleFailed(TEXT("Network connection failed"));
}

void UAsyncTaskThirdwebEngineGetTransactionReceipt::HandleFailed(const FString& Error)
{
	Failed.Broadcast(FThirdwebEngineTransactionReceipt(), Error);
	SetReadyToDestroy();
}
