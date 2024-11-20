// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "Containers/ThirdwebIPFSUploadResult.h"

#include "ThirdwebRuntimeSettings.h"
#include "ThirdwebUtils.h"

FThirdwebIPFSUploadResult FThirdwebIPFSUploadResult::FromJson(const FString& JsonString)
{
	return FromJson(ThirdwebUtils::Json::ToJson(JsonString));
}

FThirdwebIPFSUploadResult FThirdwebIPFSUploadResult::FromJson(const TSharedPtr<FJsonObject>& JsonObject)
{
	FThirdwebIPFSUploadResult Result;
	if (JsonObject.IsValid())
	{
		if (JsonObject->HasTypedField<EJson::String>(TEXT("ipfsHash")))
		{
			Result.Hash = JsonObject->GetStringField(TEXT("ipfsHash"));
			Result.PreviewUrl = FString::Printf(TEXT("https://%s.ipfscdn.io/ipfs/%s"), *UThirdwebRuntimeSettings::GetClientId(), *Result.Hash);
		}
		if (JsonObject->HasTypedField<EJson::String>(TEXT("pinSize")))
		{
			Result.PinSize = JsonObject->GetStringField(TEXT("pinSize"));
		}
		if (JsonObject->HasTypedField<EJson::String>(TEXT("ipfsHash")))
		{
			Result.Timestamp = FDateTime::FromUnixTimestamp(ThirdwebUtils::Internal::ParseInt64(JsonObject->GetStringField(TEXT("ipfsHash"))));
		}
	}
	return Result;
}
