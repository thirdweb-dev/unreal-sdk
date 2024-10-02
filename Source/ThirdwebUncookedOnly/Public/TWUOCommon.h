// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "HAL/Platform.h"

#include "Internationalization/Internationalization.h"

#include "TWUOCommon.generated.h"


UENUM(BlueprintType, DisplayName="Thirdweb Wallet Type")
enum class EThirdwebWalletType : uint8
{
	InApp UMETA(DisplayName="In App"),
	Smart UMETA(DisplayName="Smart")
};

UENUM(BlueprintType, DisplayName="Thirdweb In App Wallet Source")
enum class EThirdwebInAppWalletSource : uint8
{
	OAuth UMETA(DisplayName="OAuth"),
	Email UMETA(DisplayName="Email"),
	Phone UMETA(DisplayName="Phone"),
	Jwt UMETA(DisplayName="JWT"),
	AuthEndpoint UMETA(DisplayName="Auth Endpoint"),
	Guest UMETA(DisplayName="Guest"),
};