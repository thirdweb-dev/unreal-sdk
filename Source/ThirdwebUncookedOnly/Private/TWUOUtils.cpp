// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "TWUOUtils.h"

#define LOCTEXT_NAMESPACE "ThirdwebUncookedOnly"

namespace ThirdwebUtils
{
	namespace Maps
	{
		const TMap<EThirdwebInAppWalletSource, FText> InAppWalletSourceToText = {
			{EThirdwebInAppWalletSource::OAuth, LOCTEXT("OAuth", "OAuth")},
			{EThirdwebInAppWalletSource::Email, LOCTEXT("Email", "Email")},
			{EThirdwebInAppWalletSource::Phone, LOCTEXT("Phone", "Phone")},
			{EThirdwebInAppWalletSource::Jwt, LOCTEXT("Jwt", "Jwt")},
			{EThirdwebInAppWalletSource::AuthEndpoint, LOCTEXT("Auth Endpoint", "Auth Endpoint")},
			{EThirdwebInAppWalletSource::Guest, LOCTEXT("Guest", "Guest")},
			{EThirdwebInAppWalletSource::Siwe, LOCTEXT("SIWE", "SIWE")},
		};
	}

	EThirdwebInAppWalletSource ToInAppWalletSource(const FString& Source)
	{
		for (const auto& It : Maps::InAppWalletSourceToText)
		{
			if (It.Value.ToString().Equals(Source, ESearchCase::IgnoreCase))
			{
				return It.Key;
			}
		}
		return EThirdwebInAppWalletSource::OAuth;
	}

	EThirdwebInAppWalletSource ToInAppWalletSource(const FText& Source)
	{
		return ToInAppWalletSource(Source.ToString());
	}

	FString ToString(const EThirdwebInAppWalletSource& Source)
	{
		return Maps::InAppWalletSourceToText.Contains(Source) ? Maps::InAppWalletSourceToText[Source].ToString() : TEXT("Invalid");
	}

	FText ToText(const EThirdwebInAppWalletSource& Source)
	{
		return Maps::InAppWalletSourceToText.Contains(Source) ? Maps::InAppWalletSourceToText[Source] : LOCTEXT("Invalid", "Invalid");
	}
}

#undef LOCTEXT_NAMESPACE