// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Utils/AsyncTaskThirdwebFetchIpfsData.h"

#include "ThirdwebUtils.h"
#include "Components/SlateWrapperTypes.h"

UAsyncTaskThirdwebFetchIpfsData* UAsyncTaskThirdwebFetchIpfsData::FetchIpfsData(UObject* WorldContextObject,const FString& Uri)
{
	NEW_TASK
	Task->Uri = Uri;
	RR_TASK
}

void UAsyncTaskThirdwebFetchIpfsData::Activate()
{
	ThirdwebUtils::Storage::Download(
		Uri,
		BIND_UOBJECT_DELEGATE(ThirdwebUtils::Storage::FDownloadBytesSuccessDelegate, HandleResponse),
		BIND_UOBJECT_DELEGATE(FStringDelegate, HandleFailed)
	);
}

void UAsyncTaskThirdwebFetchIpfsData::HandleResponse(const TArray<uint8>& Data)
{
	Success.Broadcast(Data, TEXT(""));
	SetReadyToDestroy();
}

void UAsyncTaskThirdwebFetchIpfsData::HandleFailed(const FString& Error)
{
	Failed.Broadcast({}, Error);
	SetReadyToDestroy();
}
