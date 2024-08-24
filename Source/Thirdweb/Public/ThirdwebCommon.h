// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once
#include "HAL/Platform.h"
#include "Internationalization/Internationalization.h"
#include "ThirdwebCommon.generated.h"

#define LOCTEXT_NAMESPACE "Thirdweb"

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

inline FText ToText(const EThirdwebOAuthProvider Provider)
{
	static TMap<EThirdwebOAuthProvider, FText> Map = {
		{EThirdwebOAuthProvider::Google, LOCTEXT("Google", "Google")},
		{EThirdwebOAuthProvider::Apple, LOCTEXT("Apple", "Apple")},
		{EThirdwebOAuthProvider::Facebook, LOCTEXT("Facebook", "Facebook")}
	};
	return Map.Contains(Provider) ? Map[Provider] : FText::FromString(TEXT("Invalid"));
}

inline FString ToString(const EThirdwebOAuthProvider Provider)
{
	return ToText(Provider).ToString();
}

#undef LOCTEXT_NAMESPACE

