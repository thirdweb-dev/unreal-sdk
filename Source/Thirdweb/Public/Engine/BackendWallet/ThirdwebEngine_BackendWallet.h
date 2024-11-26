// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "ThirdwebMacros.h"

struct FThirdwebBackendWallet;
struct FThirdwebURLSearchParams;

namespace ThirdwebEngine::BackendWallet
{
	extern FString FormatUrl(const FString& Endpoint, const FThirdwebURLSearchParams& Params);

	DECLARE_DELEGATE_OneParam(FGetAllDelegate, const TArray<FThirdwebBackendWallet>& /* BackendWallets */)
	extern void GetAll(
		const UObject* Outer,
		const int32 Page,
		const int32 Limit,
		const FGetAllDelegate& SuccessDelegate,
		const FStringDelegate& ErrorDelegate
	);
}
