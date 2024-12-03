// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "TWUOFunctionLibrary.h"

#include "TWUOUtils.h"

FString UTWUOFunctionLibrary::Conv_ThirdwebInAppWalletSourceToString(EThirdwebInAppWalletSource Source)
{
	return ThirdwebUtils::ToString(Source);
}

EThirdwebInAppWalletSource UTWUOFunctionLibrary::Conv_StringToThirdwebInAppWalletSource(FString Source)
{
	return ThirdwebUtils::ToInAppWalletSource(Source);
}
