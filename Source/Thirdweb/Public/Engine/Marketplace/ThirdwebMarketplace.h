// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "UObject/Object.h"
#include "ThirdwebMarketplace.generated.h"

UCLASS(Blueprintable, BlueprintType)
class THIRDWEB_API UThirdwebMarketplace : public UObject
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(ExposeOnSpawn), Category="Details")
	FString ContractAddress;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(ExposeOnSpawn), DisplayName="Chain ID", Category="Details")
	int64 ChainId;

public:
	UFUNCTION(BlueprintPure, DisplayName="Get Chain ID", Category="Thirdweb|Marketplace|Details")
	int64 GetChainId() const { return ChainId; }
	
	UFUNCTION(BlueprintPure, DisplayName="Get Contract Address", Category="Thirdweb|Marketplace|Details")
	FString GetContractAddress() const { return ContractAddress; }

	virtual bool IsValid() { return ChainId > 0 && !ContractAddress.IsEmpty(); }
};
