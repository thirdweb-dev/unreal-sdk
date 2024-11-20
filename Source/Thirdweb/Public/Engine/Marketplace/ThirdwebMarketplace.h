// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "UObject/Object.h"
#include "ThirdwebMarketplace.generated.h"

UCLASS()
class THIRDWEB_API UThirdwebMarketplace : public UObject
{
	GENERATED_BODY()

public:

	int64 GetChainId() const { return ChainId; }
	
protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Details")
	FString ContractAddress;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, DisplayName="Chain ID", Category="Details")
	int64 ChainId;
	
};
