// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/AsyncTaskThirdwebFetchIpfsJson.h"

#include "ThirdwebUtils.h"
#include "Components/SlateWrapperTypes.h"

UAsyncTaskThirdwebFetchIpfsJson* UAsyncTaskThirdwebFetchIpfsJson::FetchIpfsJson(
	UObject* WorldContextObject,
	const FString& Uri
)
{
	NEW_TASK
	Task->Uri = Uri;
	RR_TASK
}

void UAsyncTaskThirdwebFetchIpfsJson::Activate()
{
	ThirdwebUtils::Storage::Download(
		Uri,
		BIND_UOBJECT_DELEGATE(ThirdwebUtils::Storage::FDownloadBytesSuccessDelegate, HandleResponse),
		BIND_UOBJECT_DELEGATE(FStringDelegate, HandleFailed)
	);
}

void UAsyncTaskThirdwebFetchIpfsJson::HandleResponse(const TArray<uint8>& Result)
{
	Success.Broadcast(ThirdwebUtils::Storage::ConvertDownloadResult<FString>(Result), TEXT(""));
	SetReadyToDestroy();
}

void UAsyncTaskThirdwebFetchIpfsJson::HandleFailed(const FString& Error)
{
	Failed.Broadcast(TEXT(""), Error);
	SetReadyToDestroy();
}
