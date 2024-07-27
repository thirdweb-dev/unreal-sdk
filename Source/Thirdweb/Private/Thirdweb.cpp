// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "Thirdweb.h"

#include "ThirdwebLog.h"

void Thirdweb::FFIResult::ToOperationResult(bool& Success, bool& CanRetry, FString& Output)
{
	// Log FFIResult object
	TW_LOG(Log, TEXT("FFIResult: success=%s, message=%s"), success ? TEXT("true") : TEXT("false"), Message());

	Success = success;
	CanRetry = false;
	Output = Message();
	
	if (Success && Output.StartsWith("RecoverableError"))
	{
		Success = false;
		CanRetry = true;
		Output = Output.Mid(17).TrimStart();
	}
	
	free_ffi_result(*this);
}
