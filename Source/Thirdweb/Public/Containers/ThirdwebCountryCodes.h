// Copyright (c) 2024 Thirdweb. All Rights Reserved.

// Warning: The information provided in this file is for convenience, and is subject to change at any time.

#pragma once

#include "ThirdwebCountryCodes.generated.h"

USTRUCT(BlueprintType, DisplayName="Country Code")
struct THIRDWEB_API FThirdwebCountryCode
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Country Code")
	int32 Code = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Country Code")
	FText Abbreviation;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Country Code")
	FText FullName;

	bool operator==(const FThirdwebCountryCode& Other) const
	{
		return Code == Other.Code;
	}

	bool operator!=(const FThirdwebCountryCode& Other) const
	{
		return Code != Other.Code;
	}
	
	friend uint32 GetTypeHash(const FThirdwebCountryCode& Other)
	{
		return GetTypeHash(Other.Code);
	}
};

namespace ThirdwebCountryCodes
{
	extern const TMap<int32, FThirdwebCountryCode> Map;
	extern FThirdwebCountryCode GetCountryCodeData(const int32 CountryCode);
	extern TArray<FThirdwebCountryCode> GetCountryCodesArray();
}
