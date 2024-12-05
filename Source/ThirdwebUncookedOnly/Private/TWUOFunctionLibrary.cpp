// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "TWUOFunctionLibrary.h"

#include "TWUOUtils.h"

FString UTWUOFunctionLibrary::Conv_ThirdwebInAppWalletSourceToString(EThirdwebInAppWalletSource Source)
{
	return ThirdwebUtils::ToString(Source);
}

FText UTWUOFunctionLibrary::Conv_ThirdwebInAppWalletSourceToText(EThirdwebInAppWalletSource Source)
{
	return ThirdwebUtils::ToText(Source);
}

EThirdwebInAppWalletSource UTWUOFunctionLibrary::Conv_StringToThirdwebInAppWalletSource(FString Source)
{
	return ThirdwebUtils::ToInAppWalletSource(Source);
}

EThirdwebInAppWalletSource UTWUOFunctionLibrary::Conv_TextToThirdwebInAppWalletSource(FText Source)
{
	return ThirdwebUtils::ToInAppWalletSource(Source);
}
