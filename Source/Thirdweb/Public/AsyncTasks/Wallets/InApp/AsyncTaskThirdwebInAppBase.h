// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "AsyncTasks/AsyncTaskThirdwebBase.h"

#include "Wallets/ThirdwebInAppWalletHandle.h"

#include "AsyncTaskThirdwebInAppBase.generated.h"


/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class THIRDWEB_API UAsyncTaskThirdwebInAppBase : public UAsyncTaskThirdwebBase
{
	GENERATED_BODY()

protected:	
	UPROPERTY(Transient)
	FInAppWalletHandle InAppWallet;
};
