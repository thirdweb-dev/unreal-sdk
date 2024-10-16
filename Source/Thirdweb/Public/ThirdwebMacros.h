// Copyright (c) 2024 Thirdweb. All Rights Reserved.

// ReSharper disable CppUE4CodingStandardNamingViolationWarning,CppUE4CodingStandardNamingViolationWarning
#pragma once

#include "Delegates/DelegateCombinations.h"

#define TO_RUST_STRING(v) v.IsEmpty() ? nullptr : TCHAR_TO_UTF8(*v)

#define TO_RUST_TIMESTAMP(v) v == FDateTime::MinValue() ? 0 : v.ToUnixTimestamp()

DECLARE_DELEGATE_OneParam(FStringDelegate, const FString&);
DECLARE_DELEGATE_OneParam(FStringArrayDelegate, const TArray<FString>&);

DECLARE_DELEGATE_OneParam(FBoolDelegate, const bool&);