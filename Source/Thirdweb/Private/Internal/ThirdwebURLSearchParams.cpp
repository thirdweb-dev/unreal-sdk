// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "Internal/ThirdwebURLSearchParams.h"

#include "GenericPlatform/GenericPlatformHttp.h"

void FThirdwebURLSearchParams::Append(const FString& Key, const FString& Value)
{
	Params.FindOrAdd(Key).Add(Value);
}

FString FThirdwebURLSearchParams::Get(const FString& Key) const
{
	if (const TArray<FString>* Values = Params.Find(Key))
	{
		return Values->Num() > 0 ? (*Values)[0] : TEXT("");
	}
	return TEXT("");
}

TArray<FString> FThirdwebURLSearchParams::GetAll(const FString& Key) const
{
	if (const TArray<FString>* Values = Params.Find(Key))
	{
		return *Values;
	}
	return TArray<FString>();
}

bool FThirdwebURLSearchParams::Has(const FString& Key) const
{
	return Params.Contains(Key);
}

void FThirdwebURLSearchParams::SetIf(const bool Condition, const FString& Key, const FString& Value)
{
	if (Condition)
	{
		Params.FindOrAdd(Key).Empty(1);
		Params[Key].Add(Value);
	}
}

void FThirdwebURLSearchParams::Delete(const FString& Key)
{
	Params.Remove(Key);;
}

FString FThirdwebURLSearchParams::ToString(const bool bIncludeSeparator) const
{
	TArray<FString> QueryStringComponents;
	for (const TTuple<FString, TArray<FString>>& Elem : Params)
	{
		for (const FString& Value : Elem.Value)
		{
			QueryStringComponents.Add(FString::Printf(TEXT("%s=%s"), *EncodeURIComponent(Elem.Key), *EncodeURIComponent(Value)));
		}
	}
	FString Separator = bIncludeSeparator && QueryStringComponents.Num() > 0 ? TEXT("?") : TEXT("");
	return Separator + FString::Join(QueryStringComponents, TEXT("&"));
}

void FThirdwebURLSearchParams::ParseQueryString(const FString& QueryString)
{
	TArray<FString> Pairs;
	QueryString.ParseIntoArray(Pairs, TEXT("&"), true);
	for (const FString& Pair : Pairs)
	{
		FString Key, Value;
		if (Pair.Split(TEXT("="), &Key, &Value))
		{
			Append(DecodeURIComponent(Key), DecodeURIComponent(Value));
		}
	}
}

FString FThirdwebURLSearchParams::EncodeURIComponent(const FString& Component)
{
	FString Encoded = Component;
	Encoded = FGenericPlatformHttp::UrlEncode(Encoded);
	return Encoded.Replace(TEXT("+"), TEXT("%20"));
}

FString FThirdwebURLSearchParams::DecodeURIComponent(const FString& Component)
{
	FString Decoded = Component;
	Decoded = FGenericPlatformHttp::UrlDecode(Decoded);
	return Decoded.Replace(TEXT("%20"), TEXT("+"));
}

FThirdwebURLSearchParams FThirdwebURLSearchParams::Create(const FString& QueryString)
{
	FThirdwebURLSearchParams Params;
	if (!QueryString.IsEmpty())
	{
		Params.ParseQueryString(QueryString);
	}
	return Params;
}

FThirdwebURLSearchParams FThirdwebURLSearchParams::Create(const TMap<FString, TArray<FString>>& InParams)
{
	FThirdwebURLSearchParams Params;
	Params.Params = InParams;
	return Params;
}

FThirdwebURLSearchParams FThirdwebURLSearchParams::Create(const TMap<FString, FString>& InParams)
{
	FThirdwebURLSearchParams Params;
	for (const TPair<FString, FString>& Pair : InParams)
	{
		Params.Params.Emplace(Pair.Key, {Pair.Value});
	}
	return Params;
}