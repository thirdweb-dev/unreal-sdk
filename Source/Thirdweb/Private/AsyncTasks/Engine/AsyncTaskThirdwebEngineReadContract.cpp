// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Engine/AsyncTaskThirdwebEngineReadContract.h"

#include "HttpModule.h"
#include "ThirdwebLog.h"
#include "ThirdwebRuntimeSettings.h"
#include "ThirdwebUtils.h"

#include "Dom/JsonObject.h"

#include "Interfaces/IHttpResponse.h"

#include "Kismet/KismetStringLibrary.h"

#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

UAsyncTaskThirdwebEngineReadContract* UAsyncTaskThirdwebEngineReadContract::ReadContract(
	UObject* WorldContextObject,
	const int64 ChainId,
	const FString& ContractAddress,
	const FString& FunctionName,
	const TArray<FString>& Args
)
{
	if (!WorldContextObject)
	{
		return nullptr;
	}
	NEW_TASK
	Task->ChainId = ChainId;
	Task->ContractAddress = ContractAddress;
	Task->FunctionName = FunctionName;
	Task->Args = Args;
	Task->RegisterWithGameInstance(WorldContextObject);
	return Task;
}

void UAsyncTaskThirdwebEngineReadContract::Activate()
{
	FHttpModule& HttpModule = FHttpModule::Get();
	const TSharedRef<IHttpRequest> Request = HttpModule.CreateRequest();
	Request->SetVerb(TEXT("GET"));
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->SetHeader("authorization", TEXT("Bearer ") + UThirdwebRuntimeSettings::GetEngineAccessToken());
	Request->SetURL(
		FString::Format(
			TEXT("{0}/contract/{1}/{2}/read?functionName={3}{4}"),
			{
				UThirdwebRuntimeSettings::GetEngineBaseUrl(),
				FString::Printf(TEXT("%lld"), ChainId),
				ContractAddress,
				FunctionName,
				Args.Num() > 0 ? TEXT("&args={0}") + UKismetStringLibrary::JoinStringArray(Args, TEXT(",")) : TEXT("")
			}
		)
	);
	ThirdwebUtils::Internal::LogRequest(Request, {UThirdwebRuntimeSettings::GetEngineAccessToken()});
	Request->SetTimeout(30.0f);
	Request->OnProcessRequestComplete().BindUObject(this, &ThisClass::HandleResponse);
	Request->ProcessRequest();
}

void UAsyncTaskThirdwebEngineReadContract::HandleResponse(FHttpRequestPtr, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	if (bConnectedSuccessfully)
	{
		FString Content = Response->GetContentAsString();
		TW_LOG(Verbose, TEXT("UAsyncTaskThirdwebEngineReadContract::HandleResponse::Content=%s"), *Content)
		TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
		const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Content);
		FJsonSerializer::Deserialize(Reader, JsonObject);
		if (JsonObject.IsValid())
		{
			if (JsonObject->HasTypedField<EJson::String>(TEXT("error")))
			{
				FString Error = TEXT("Unknown Error");
				if (JsonObject->HasTypedField<EJson::String>(TEXT("message")))
				{
					Error = JsonObject->GetStringField(TEXT("message"));
				}
				return HandleFailed(Error);
			}
			Success.Broadcast(Content, TEXT(""));
			SetReadyToDestroy();
		}
		else return HandleFailed(TEXT("Invalid Response"));
	}
	else return HandleFailed(TEXT("Network connection failed"));
}

void UAsyncTaskThirdwebEngineReadContract::HandleFailed(const FString& Error)
{
	Failed.Broadcast(TEXT(""), Error);
	SetReadyToDestroy();
}
