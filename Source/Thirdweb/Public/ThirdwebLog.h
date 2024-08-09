// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Logging/LogMacros.h"

DECLARE_LOG_CATEGORY_EXTERN(LogThirdweb, Log, All);

#define TW_LOG(Verbosity, Format, ...) UE_LOG(LogThirdweb, Verbosity, Format, ##__VA_ARGS__)