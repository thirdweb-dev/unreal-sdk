// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "Thirdweb.h"

#include "ThirdwebCommon.h"
#include "ThirdwebLog.h"

bool Thirdweb::FFIResult::AssignResult(bool& bCanRetry, FString& Output, bool bErrorOnlyResult)
{
	// Log FFIResult object
	TW_LOG(Log, TEXT("FFIResult: success=%s, message=%s"), success ? TEXT("true") : TEXT("false"), Message());

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

EFunctionResult Thirdweb::FFIResult::AssignBPResult(FString& Output)
{
	bool bCanRetry = false;
	bool bSuccess = AssignResult(bCanRetry, Output);
	return bSuccess ? EFunctionResult::Success : bCanRetry ? EFunctionResult::FailedCanRetry : EFunctionResult::Failed;
}
