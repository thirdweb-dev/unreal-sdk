// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "Engine/ThirdwebEngineTransactionOverrides.h"

#include "Dom/JsonObject.h"

TSharedPtr<FJsonObject> FThirdwebEngineTransactionOverrides::ToJson() const
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	if (Gas > 0)
	{
		JsonObject->SetStringField(TEXT("gas"), FString::Printf(TEXT("%lld"), Gas));
	}
	if (MaxFeePerGas > 0)
	{
		JsonObject->SetStringField(TEXT("maxFeePerGas"), FString::Printf(TEXT("%lld"), MaxFeePerGas));
	}
	if (MaxPriorityFeePerGas > 0)
	{
		JsonObject->SetStringField(TEXT("maxPriorityFeePerGas"), FString::Printf(TEXT("%lld"), MaxPriorityFeePerGas));
	}
	if (Value > 0)
	{
		JsonObject->SetStringField(TEXT("value"), FString::Printf(TEXT("%lld"), Value));
	}
	return JsonObject;
}
