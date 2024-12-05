// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "TWUOCommon.h"

namespace ThirdwebUtils
{
	namespace Maps
	{
		extern const TMap<EThirdwebInAppWalletSource, FText> InAppWalletSourceToText;
	}

	extern THIRDWEBUNCOOKEDONLY_API EThirdwebInAppWalletSource ToInAppWalletSource(const FString& Source);
	extern THIRDWEBUNCOOKEDONLY_API EThirdwebInAppWalletSource ToInAppWalletSource(const FText& Source);
	extern THIRDWEBUNCOOKEDONLY_API FString ToString(const EThirdwebInAppWalletSource& Source);
	extern THIRDWEBUNCOOKEDONLY_API FText ToText(const EThirdwebInAppWalletSource& Source);
}
