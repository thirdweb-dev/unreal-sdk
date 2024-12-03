// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "ThirdwebMarketplaceCommon.generated.h"

UENUM()
enum class EThirdwebMarketplaceListingStatus : uint8
{
	Invalid UMETA(DisplayName="Invalid"),
	Unset UMETA(DisplayName="Unset"),
	Created UMETA(DisplayName="Created"),
	Completed UMETA(DisplayName="Completed"),
	Cancelled UMETA(DisplayName="Cancelled"),
	Active UMETA(DisplayName="Active"),
	Expired UMETA(DisplayName="Expired"),
	Max UMETA(Hidden)
};
