// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "HAL/Platform.h"

#include "Internationalization/Internationalization.h"

#include "ThirdwebCommon.generated.h"

UENUM()
enum class EFunctionResult : uint8
{
	Success,
	Failed
};

UENUM()
enum class ESmartWalletDeployedFunctionResult : uint8
{
	Deployed,
	NotDeployed,
	Failed
};

UENUM()
enum class EOTPVerificationFunctionResult : uint8
{
	Verified,
	Retry,
	Failed
};

UENUM(Blueprintable)
enum class EThirdwebOAuthProvider : uint8
{
	Google UMETA(DisplayName="Google"),
	Apple UMETA(DisplayName="Apple"),
	Facebook UMETA(DisplayName="Facebook")
};

UENUM()
enum class EThirdwebAuthenticationMethod : uint8
{
	ClientID UMETA(DisplayName="Client ID"),
	SecretKey UMETA(DisplayName="Secret Key"),
};