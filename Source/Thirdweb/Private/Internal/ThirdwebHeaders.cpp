// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "Internal/ThirdwebHeaders.h"

#include "Interfaces/IHttpRequest.h"

FThirdwebHeaders::FThirdwebHeaders()
{
}

void FThirdwebHeaders::Set(const FString& Name, const FString& Value, const bool bCondition)
{
	if (bCondition && !Value.IsEmpty())
	{
		Headers.Add(Name, Value);
	}
}

bool FThirdwebHeaders::Remove(const FString& Name)
{
	return Headers.Remove(Name) > 0;
}

FString FThirdwebHeaders::Get(const FString& Name) const
{
	if (const FString* Value = Headers.Find(Name))
	{
		return *Value;
	}
	return TEXT("");
}

const TMap<FString, FString>& FThirdwebHeaders::GetAll() const
{
	return Headers;
}

void FThirdwebHeaders::UpdateRequest(const TSharedPtr<IHttpRequest>& Request)
{
	if (Request.IsValid())
	{
		TArray<FString> Keys;
		Headers.GenerateKeyArray(Keys);
		for (int i = 0; i < Keys.Num(); i++)
		{
			Request->SetHeader(Keys[i], Headers[Keys[i]]);
		}
	}
}
