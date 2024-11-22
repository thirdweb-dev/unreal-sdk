// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "Engine/ThirdwebEngine.h"

#include "ThirdwebRuntimeSettings.h"
#include "Internal/ThirdwebURLSearchParams.h"

namespace ThirdwebEngine
{
	FString FormatUrl(const FString& Base, const FString& Endpoint, const FThirdwebURLSearchParams& Params)
	{
		return FString::Format(TEXT("{0}/{1}/{2}{3}"), {UThirdwebRuntimeSettings::GetEngineBaseUrl(), Base, Endpoint, Params.ToString(true)});
	}
}
