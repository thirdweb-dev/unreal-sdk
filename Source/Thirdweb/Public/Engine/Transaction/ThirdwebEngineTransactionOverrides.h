// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "ThirdwebEngineTransactionOverrides.generated.h"

class FJsonObject;

USTRUCT(BlueprintType, DisplayName="Transaction Overrides")
struct THIRDWEB_API FThirdwebEngineTransactionOverrides
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, DisplayName="Gas Limit", Category="Thirdweb|Engine")
	int64 Gas = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Thirdweb|Engine")
	int64 MaxFeePerGas = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Thirdweb|Engine")
	int64 MaxPriorityFeePerGas = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Thirdweb|Engine")
	int64 Value = 0;

	bool IsDefault() const { return Gas == 0 && MaxFeePerGas == 0 && MaxPriorityFeePerGas == 0 && Value == 0; }
	TSharedPtr<FJsonObject> ToJson() const;
};
