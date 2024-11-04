// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Engine/AsyncTaskThirdwebEngineGetTransactionStatus.h"

#include "HttpModule.h"
#include "ThirdwebLog.h"
#include "ThirdwebRuntimeSettings.h"
#include "ThirdwebUtils.h"

#include "Dom/JsonObject.h"

#include "Engine/ThirdwebEngineTransactionStatusResult.h"

#include "Interfaces/IHttpResponse.h"

#include "Kismet/KismetStringLibrary.h"

#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

UAsyncTaskThirdwebEngineGetTransactionStatus* UAsyncTaskThirdwebEngineGetTransactionStatus::GetTransactionStatus(UObject* WorldContextObject, const FString& QueueID)
{
	if (!WorldContextObject)
	{
		return nullptr;
	}
	NEW_TASK
	Task->QueueId = QueueID;
	RR_TASK
}

void UAsyncTaskThirdwebEngineGetTransactionStatus::Activate()
{
	const TSharedRef<IHttpRequest> Request = ThirdwebUtils::Internal::CreateEngineRequest();
	Request->SetURL(FString::Printf(TEXT("%s/transaction/status/%s"), *UThirdwebRuntimeSettings::GetEngineBaseUrl(), *QueueId));
	ThirdwebUtils::Internal::LogRequest(Request, {UThirdwebRuntimeSettings::GetEngineAccessToken()});
	Request->OnProcessRequestComplete().BindUObject(this, &ThisClass::HandleResponse);
	Request->ProcessRequest();
}

void UAsyncTaskThirdwebEngineGetTransactionStatus::HandleResponse(FHttpRequestPtr, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	if (bConnectedSuccessfully)
	{
		FString Content = Response->GetContentAsString();
		TW_LOG(Verbose, TEXT("UAsyncTaskThirdwebEngineGetTransactionStatus::HandleResponse::Content=%s"), *Content)
		FString Error;
		if (TSharedPtr<FJsonObject> JsonObject; ThirdwebUtils::Json::ParseEngineResponse(Content, JsonObject, Error))
		{
			Success.Broadcast(FThirdwebEngineTransactionStatusResult::FromJson(JsonObject), TEXT(""));
			SetReadyToDestroy();
		}
		else return HandleFailed(Error);
	}
	else return HandleFailed(TEXT("Network connection failed"));
}

void UAsyncTaskThirdwebEngineGetTransactionStatus::HandleFailed(const FString& Error)
{
	Failed.Broadcast(FThirdwebEngineTransactionStatusResult(), Error);
	SetReadyToDestroy();
}
