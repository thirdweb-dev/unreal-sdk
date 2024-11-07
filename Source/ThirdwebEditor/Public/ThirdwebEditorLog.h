// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "Logging/LogMacros.h"
#include "Logging/LogVerbosity.h"

DECLARE_LOG_CATEGORY_EXTERN(LogThirdwebEditor, Log, All);

#define TWE_LOG(Verbosity, Format, ...) UE_LOG(LogThirdwebEditor, Verbosity, Format, ##__VA_ARGS__)