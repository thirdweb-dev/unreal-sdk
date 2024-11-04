// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "Misc/DateTime.h"

#include "ThirdwebSigner.generated.h"

USTRUCT(BlueprintType)
struct THIRDWEB_API FSigner
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Thirdweb|Wallets|Smart Wallet|Signer")
	FString Address;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Thirdweb|Wallets|Smart Wallet|Signer")
	TArray<FString> ApprovedTargets;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Thirdweb|Wallets|Smart Wallet|Signer")
	FString NativeTokenLimitPerTransaction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Thirdweb|Wallets|Smart Wallet|Signer")
	FDateTime StartTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Thirdweb|Wallets|Smart Wallet|Signer")
	FDateTime EndTime;

	static FSigner FromJson(const TSharedPtr<class FJsonObject>& JsonObject);

	bool operator==(const FSigner& Other) const
	{
		return Address.Equals(Other.Address, ESearchCase::IgnoreCase);
	}

	bool operator!=(const FSigner& Other) const
	{
		return !Address.Equals(Other.Address, ESearchCase::IgnoreCase);
	}
	
	friend uint32 GetTypeHash(const FSigner& Other)
	{
		return GetTypeHash(Other.Address);
	}
};
