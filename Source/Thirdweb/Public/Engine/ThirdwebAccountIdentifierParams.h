// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "Misc/DateTime.h"
#include "Wallets/ThirdwebSmartWalletHandle.h"
#include "ThirdwebAccountIdentifierParams.generated.h"

USTRUCT(BlueprintType, DisplayName="Account Identifier Params")
struct THIRDWEB_API FThirdwebAccountIdentifierParams
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Params")
	FSmartWalletHandle SmartWallet;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Params")
	FString FactoryAddress;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Params")
	FString Salt;
};
