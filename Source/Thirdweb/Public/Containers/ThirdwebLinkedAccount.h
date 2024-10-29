// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "ThirdwebLinkedAccount.generated.h"

USTRUCT(BlueprintType, DisplayName="Linked Account")
struct THIRDWEB_API FThirdwebLinkedAccount
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Thirdweb|Wallets|InApp")
	FName Id;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Thirdweb|Wallets|InApp")
	FString Type;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Thirdweb|Wallets|InApp")
	FString Email;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Thirdweb|Wallets|InApp")
	FString Address;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Thirdweb|Wallets|InApp")
	FString Phone;

	static FThirdwebLinkedAccount FromJson(const TSharedPtr<class FJsonObject>& JsonObject);

	bool operator==(const FThirdwebLinkedAccount& Other) const
	{
		return Id.IsEqual(Other.Id);
	}

	bool operator!=(const FThirdwebLinkedAccount& Other) const
	{
		return !Id.IsEqual(Other.Id);
	}

	friend uint32 GetTypeHash(const FThirdwebLinkedAccount& Other)
	{
		return GetTypeHash(Other.Id);
	}
};
