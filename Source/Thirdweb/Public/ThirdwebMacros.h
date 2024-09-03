// Copyright (c) 2024 Thirdweb. All Rights Reserved.

// ReSharper disable CppUE4CodingStandardNamingViolationWarning,CppUE4CodingStandardNamingViolationWarning
#pragma once

#define TO_RUST_STRING(v) v.IsEmpty() ? nullptr : TCHAR_TO_UTF8(*v)

#define TO_RUST_TIMESTAMP(v) v == FDateTime::MinValue() ? 0 : v.ToUnixTimestamp()