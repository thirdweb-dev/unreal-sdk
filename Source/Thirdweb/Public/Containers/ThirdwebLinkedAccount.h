// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "Dom/JsonObject.h"

#include "Misc/DateTime.h"

#include "ThirdwebLinkedAccount.generated.h"

USTRUCT(BlueprintType)
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

	static FThirdwebLinkedAccount FromJson(const TSharedPtr<FJsonObject>& JsonObject);
};
