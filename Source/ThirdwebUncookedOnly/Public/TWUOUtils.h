// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once
#include "TWUOCommon.h"

namespace ThirdwebUtils
{
	namespace Maps
	{
		static const TMap<EThirdwebInAppWalletSource, FString> InAppWalletSourceToString = {
			{EThirdwebInAppWalletSource::OAuth, TEXT("OAuth")},
			{EThirdwebInAppWalletSource::Email, TEXT("Email")},
			{EThirdwebInAppWalletSource::Phone, TEXT("Phone")},
			{EThirdwebInAppWalletSource::Jwt, TEXT("Jwt")},
			{EThirdwebInAppWalletSource::AuthEndpoint, TEXT("AuthEndpoint")},
			{EThirdwebInAppWalletSource::Guest, TEXT("Guest")},
		};
	}

	static EThirdwebInAppWalletSource ToInAppWalletSource(const FString& Source)
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
}
