// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "Containers/ThirdwebSigner.h"

#include "Dom/JsonObject.h"

FSigner FSigner::FromJson(const TSharedPtr<FJsonObject>& JsonObject)
{
	FSigner Signer;
	if (JsonObject.IsValid())
	{
		if (JsonObject->HasTypedField<EJson::String>(TEXT("signer")))
		{
			Signer.Address = JsonObject->GetStringField(TEXT("signer"));
		}
		if (JsonObject->HasTypedField<EJson::Array>(TEXT("approvedTargets")))
		{
			JsonObject->TryGetStringArrayField(TEXT("approvedTargets"), Signer.ApprovedTargets);
		}
		if (JsonObject->HasTypedField<EJson::String>(TEXT("nativeTokenLimitPerTransaction")))
		{
			Signer.NativeTokenLimitPerTransaction = JsonObject->GetStringField(TEXT("nativeTokenLimitPerTransaction"));
		}
		if (JsonObject->HasTypedField<EJson::String>(TEXT("startTimestamp")))
		{
			Signer.StartTime = FDateTime::FromUnixTimestampDecimal(FCString::Atoi(*JsonObject->GetStringField(TEXT("startTimestamp"))));
		}
		if (JsonObject->HasTypedField<EJson::String>(TEXT("endTimestamp")))
		{
			Signer.EndTime = FDateTime::FromUnixTimestampDecimal(FCString::Atoi(*JsonObject->GetStringField(TEXT("endTimestamp"))));
		}
	}
	return Signer;
}
