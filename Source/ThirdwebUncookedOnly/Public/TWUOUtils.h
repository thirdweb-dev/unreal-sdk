// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "TWUOCommon.h"

namespace ThirdwebUtils
{
	namespace Maps
	{
		extern const TMap<EThirdwebInAppWalletSource, FString> InAppWalletSourceToString;
	}

	extern THIRDWEBUNCOOKEDONLY_API EThirdwebInAppWalletSource ToInAppWalletSource(const FString& Source);
	extern THIRDWEBUNCOOKEDONLY_API FString ToString(const EThirdwebInAppWalletSource& Source);
}
