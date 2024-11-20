// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "TWUOUtils.h"

namespace ThirdwebUtils
{
	namespace Maps
	{
		const TMap<EThirdwebInAppWalletSource, FString> InAppWalletSourceToString = {
			{EThirdwebInAppWalletSource::OAuth, TEXT("OAuth")},
			{EThirdwebInAppWalletSource::Email, TEXT("Email")},
			{EThirdwebInAppWalletSource::Phone, TEXT("Phone")},
			{EThirdwebInAppWalletSource::Jwt, TEXT("Jwt")},
			{EThirdwebInAppWalletSource::AuthEndpoint, TEXT("AuthEndpoint")},
			{EThirdwebInAppWalletSource::Guest, TEXT("Guest")},
			{EThirdwebInAppWalletSource::Siwe, TEXT("SIWE")}
		};
	}

	EThirdwebInAppWalletSource ToInAppWalletSource(const FString& Source)
	{
		for (const auto& It : Maps::InAppWalletSourceToString)
		{
			if (It.Value.Equals(Source, ESearchCase::IgnoreCase))
			{
				return It.Key;
			}
		}
		return EThirdwebInAppWalletSource::OAuth;
	}

	FString ToString(const EThirdwebInAppWalletSource& Source)
	{
		return Maps::InAppWalletSourceToString.Contains(Source) ? Maps::InAppWalletSourceToString[Source] : TEXT("Invalid");
	}
}
