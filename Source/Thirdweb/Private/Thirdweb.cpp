// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "Thirdweb.h"
#include "ThirdwebLog.h"

bool Thirdweb::FFIResult::AssignResult(FString& Output, bool bErrorOnlyResult) const
{
	Log();
	bool bSuccess = success;
	Output = Message();

	if (bSuccess && bErrorOnlyResult)
	{
		Output.Empty();
	}
	free_ffi_result(*this);
	return bSuccess;
}

bool Thirdweb::FFIResult::AssignRetryResult(bool& bCanRetry, FString& Output, bool bErrorOnlyResult) const
{
	Log();
	bool bSuccess = success;
	bCanRetry = false;
	Output = Message();
	
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
	FString Output = Message();
	free_ffi_result(*this);
	return Output;
}

void Thirdweb::FFIResult::Free() const
{
	free_ffi_result(*this);
}

void Thirdweb::FFIResult::Log() const
{
	TW_LOG(Log, TEXT("FFIResult: success=%s, message=%s"), success ? TEXT("true") : TEXT("false"), Message());
}
