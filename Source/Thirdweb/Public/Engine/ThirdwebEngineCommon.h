// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "ThirdwebEngineCommon.generated.h"

UENUM(BlueprintType, DisplayName="Transaction Status")
enum class EThirdwebEngineTransactionStatus : uint8
{
	Queued UMETA(DisplayName="Queued"),
	Sent UMETA(DisplayName="Sent"),
	Mined UMETA(DisplayName="Mined"),
	Errored UMETA(DisplayName="Errored"),
	Cancelled UMETA(DisplayName="Cancelled"),
	Unknown UMETA(DisplayName="Unknown", Hidden)
};

UENUM(BlueprintType, DisplayName="Transaction On Chain Status")
enum class EThirdwebEngineTransactionOnChainStatus : uint8
{
	Success UMETA(DisplayName="Success"),
	Reverted UMETA(DisplayName="Reverted"),
	Unknown UMETA(DisplayName="Unknown", Hidden)
};