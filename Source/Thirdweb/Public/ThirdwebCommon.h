// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "ThirdwebCommon.generated.h"

UENUM(BlueprintType)
enum class EFunctionResult : uint8
{
	Success,
	FailedCanRetry,
	Failed
};

UENUM(BlueprintType)
enum class ESmartWalletDeployedFunctionResult : uint8
{
	Deployed,
	NotDeployed,
	FailedCanRetry,
	Failed
};

UENUM(BlueprintType)
enum class EWalletConnectedFunctionResult : uint8
{
	Connected,
	Disconnected,
	FailedCanRetry,
	Failed
};