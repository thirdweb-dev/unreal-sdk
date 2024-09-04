// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "Thirdweb.h"

#include "ThirdwebLog.h"

bool Thirdweb::FFIResult::AssignResult(FString& Output, const bool bErrorOnlyResult) const
{
	Log();
	bool bSuccess = success;
	Output = FString(UTF8_TO_TCHAR(message));

	if (bSuccess && bErrorOnlyResult)
	{
		Output.Empty();
	}
	free_ffi_result(*this);
	return bSuccess;
}

bool Thirdweb::FFIResult::AssignRetryResult(bool& bCanRetry, FString& Output, const bool bErrorOnlyResult) const
{
	Log();
	bool bSuccess = success;
	bCanRetry = false;
	Output = FString(UTF8_TO_TCHAR(message));
	
	if (bSuccess && Output.StartsWith("RecoverableError"))
	{
		bSuccess = false;
		bCanRetry = true;
		Output = Output.Mid(17).TrimStart();
	}

	if (bSuccess && bErrorOnlyResult)
	{
		Output.Empty();
	}
	free_ffi_result(*this);
	return bSuccess;
}

FString Thirdweb::FFIResult::GetOutput() const
{
	Log();
	FString Output = FString(UTF8_TO_TCHAR(message));
	free_ffi_result(*this);
	return Output;
}

void Thirdweb::FFIResult::Free() const
{
	free_ffi_result(*this);
}

void Thirdweb::FFIResult::Log() const
{
	TW_LOG(Log, TEXT("FFIResult: success=%s, message=%s"), success ? TEXT("true") : TEXT("false"), *FString(UTF8_TO_TCHAR(message)));
}
