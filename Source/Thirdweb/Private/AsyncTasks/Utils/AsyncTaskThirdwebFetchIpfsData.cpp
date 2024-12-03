// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Utils/AsyncTaskThirdwebFetchIpfsData.h"

#include "ThirdwebUtils.h"
#include "Components/SlateWrapperTypes.h"
#include "Engine/Texture2DDynamic.h"

void UAsyncTaskThirdwebFetchIpfsBase::Activate()
{
	ThirdwebUtils::Storage::Download(
		IpfsUri,
		BIND_UOBJECT_DELEGATE(ThirdwebUtils::Storage::FDownloadBytesSuccessDelegate, HandleResponse),
		BIND_UOBJECT_DELEGATE(FStringDelegate, HandleFailed)
	);
}

void UAsyncTaskThirdwebFetchIpfsBase::HandleResponse(const TArray<uint8>& Data)
{
	FormatAndBroadcast(Data);
	SetReadyToDestroy();
}


void UAsyncTaskThirdwebFetchIpfsBase::HandleFailed(const FString& Error)
{
	SetReadyToDestroy();
}

void UAsyncTaskThirdwebFetchIpfsRaw::FormatAndBroadcast(const TArray<uint8>& Data)
{
	Success.Broadcast(Data, TEXT(""));
}

void UAsyncTaskThirdwebFetchIpfsRaw::HandleFailed(const FString& Error)
{
	Failed.Broadcast({}, Error);
	Super::HandleFailed(Error);
}

void UAsyncTaskThirdwebFetchIpfsImage::FormatAndBroadcast(const TArray<uint8>& Data)
{
	Success.Broadcast(ThirdwebUtils::Storage::ConvertDownloadResult<UTexture2DDynamic*>(Data), TEXT(""));
}

void UAsyncTaskThirdwebFetchIpfsImage::HandleFailed(const FString& Error)
{
	Failed.Broadcast(nullptr, Error);
	Super::HandleFailed(Error);
}


void UAsyncTaskThirdwebFetchIpfsJson::FormatAndBroadcast(const TArray<uint8>& Data)
{
	Success.Broadcast(ThirdwebUtils::Storage::ConvertDownloadResult<FString>(Data), TEXT(""));
}

void UAsyncTaskThirdwebFetchIpfsJson::HandleFailed(const FString& Error)
{
	Failed.Broadcast(TEXT(""), Error);
	Super::HandleFailed(Error);
}