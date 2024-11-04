// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Engine/AsyncTaskThirdwebEngineReadContract.h"

#include "HttpModule.h"
#include "ThirdwebLog.h"
#include "ThirdwebRuntimeSettings.h"
#include "ThirdwebUtils.h"

#include "Interfaces/IHttpResponse.h"

#include "Kismet/KismetStringLibrary.h"

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
	const TSharedRef<IHttpRequest> Request = ThirdwebUtils::Internal::CreateEngineRequest();
	Request->SetURL(
		FString::Format(
			TEXT("{0}/contract/{1}/{2}/read?functionName={3}{4}"),
			{
				UThirdwebRuntimeSettings::GetEngineBaseUrl(),
				FString::Printf(TEXT("%lld"), ChainId),
				ContractAddress,
				FunctionName,
				Args.Num() > 0 ? TEXT("&args=") + UKismetStringLibrary::JoinStringArray(Args, TEXT(",")) : TEXT("")
			}
		)
	);
	ThirdwebUtils::Internal::LogRequest(Request, {UThirdwebRuntimeSettings::GetEngineAccessToken()});
	Request->OnProcessRequestComplete().BindUObject(this, &ThisClass::HandleResponse);
	Request->ProcessRequest();
}

void UAsyncTaskThirdwebEngineReadContract::HandleResponse(FHttpRequestPtr, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	if (bConnectedSuccessfully)
	{
		FString Content = Response->GetContentAsString();
		TW_LOG(Verbose, TEXT("UAsyncTaskThirdwebEngineReadContract::HandleResponse::Content=%s"), *Content)
		FString Error;
		if (TSharedPtr<FJsonObject> JsonObject; ThirdwebUtils::Json::ParseEngineResponse(Content, JsonObject, Error))
		{
			Success.Broadcast(Content, TEXT(""));
			SetReadyToDestroy();
		}
		else return HandleFailed(Error);
	}
	else return HandleFailed(TEXT("Network connection failed"));
}

void UAsyncTaskThirdwebEngineReadContract::HandleFailed(const FString& Error)
{
	Failed.Broadcast(TEXT(""), Error);
	SetReadyToDestroy();
}
