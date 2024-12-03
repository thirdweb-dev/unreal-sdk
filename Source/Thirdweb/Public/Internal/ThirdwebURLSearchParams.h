// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

struct FThirdwebURLSearchParams
{
	FThirdwebURLSearchParams() = default;

private:
	TMap<FString, TArray<FString>> Params;

public:
	void Append(const FString& Key, const FString& Value);
	FString Get(const FString& Key) const;
	TArray<FString> GetAll(const FString& Key) const;
	bool Has(const FString& Key) const;

	template <typename T = FString>
	void Set(const FString& Key, const T& Value, const bool Condition)
	{
		if constexpr (std::is_same_v<T, FString>)
		{
			SetIf(Condition, Key, Value);
		}
		else if constexpr (std::is_same_v<T, int32>)
		{
			SetIf(Condition, Key, FString::FromInt(Value));
		}
		else if constexpr (std::is_same_v<T, int64>)
		{
			SetIf(Condition, Key, FString::Printf(TEXT("%lld"), Value));
		}
		else if constexpr (std::is_same_v<T, bool>)
		{
			SetIf(Condition, Key, Value ? TEXT("true") : TEXT("false"));
		}
		else
		{
			static_assert(std::is_same_v<T, FString> || std::is_same_v<T, int32> || std::is_same_v<T, int64> || std::is_same_v<T, bool>, "Unsupported type");
		}
	}

	template <typename T = FString>
	void Set(const FString& Key, const T& Value)
	{
		if constexpr (std::is_same_v<T, FString>)
		{
			SetIf(true, Key, Value);
		}
		else if constexpr (std::is_same_v<T, int32>)
		{
			SetIf(true, Key, FString::FromInt(Value));
		}
		else if constexpr (std::is_same_v<T, int64>)
		{
			SetIf(true, Key, FString::Printf(TEXT("%lld"), Value));
		}
		else if constexpr (std::is_same_v<T, bool>)
		{
			SetIf(true, Key, Value ? TEXT("true") : TEXT("false"));
		}
		else
		{
			static_assert(std::is_same_v<T, FString> || std::is_same_v<T, int32> || std::is_same_v<T, int64> || std::is_same_v<T, bool>, "Unsupported type");
		}
	}

	void Delete(const FString& Key);
	FString ToString(const bool bIncludeSeparator = false) const;

private:
	void SetIf(const bool Condition, const FString& Key, const FString& Value);
	void ParseQueryString(const FString& QueryString);
	static FString EncodeURIComponent(const FString& Component);
	static FString DecodeURIComponent(const FString& Component);

public:
	static FThirdwebURLSearchParams Create(const FString& QueryString = "");
	static FThirdwebURLSearchParams Create(const TMap<FString, TArray<FString>>& InParams = {});
	static FThirdwebURLSearchParams Create(const TMap<FString, FString>& InParams = {});
};
