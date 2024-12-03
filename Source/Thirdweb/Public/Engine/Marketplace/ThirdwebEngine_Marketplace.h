// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "ThirdwebMacros.h"
#include "Engine/Marketplace/DirectListings/ThirdwebEngine_Marketplace_DirectListings.h"
#include "Engine/Marketplace/EnglishAuctions/ThirdwebEngine_Marketplace_EnglishAuctions.h"
#include "Engine/Marketplace/Offers/ThirdwebEngine_Marketplace_Offers.h"

struct FThirdwebAccountIdentifierParams;
struct FSmartWalletHandle;
class FThirdwebHeaders;
struct FThirdwebURLSearchParams;

namespace ThirdwebEngine::Marketplace
{
	extern FString FormatUrl(const int64 ChainId, const FString& ContractAddress, const FString& Section, const FString& Endpoint, const FThirdwebURLSearchParams& Params);

	extern FThirdwebHeaders MakePostHeaders(const FString& BackendWalletAddress, const FString& IdempotencyKey, const FThirdwebAccountIdentifierParams& Account);
}
