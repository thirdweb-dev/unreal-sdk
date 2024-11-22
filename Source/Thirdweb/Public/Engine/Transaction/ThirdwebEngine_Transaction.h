// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "ThirdwebMacros.h"

struct FThirdwebEngineTransactionStatusResult;
struct FThirdwebEngineTransactionReceipt;
struct FThirdwebURLSearchParams;

namespace ThirdwebEngine::Transaction
{
	extern FString FormatUrl(const FString& Endpoint, const FThirdwebURLSearchParams& Params);
	extern FString FormatUrl(const int64 ChainId, const FString& Endpoint, const FThirdwebURLSearchParams& Params);

	DECLARE_DELEGATE_OneParam(FGetTransactionStatusDelegate, const FThirdwebEngineTransactionStatusResult& /** Result */);
	extern THIRDWEB_API void GetStatus(const UObject* Outer, const FString& QueueId, const FGetTransactionStatusDelegate& SuccessDelegate, const FStringDelegate& ErrorDelegate);

	DECLARE_DELEGATE_OneParam(FGetTransactionReceiptDelegate, const FThirdwebEngineTransactionReceipt& /** Receipt */);
	extern THIRDWEB_API void GetReceipt(
		const UObject* Outer,
		const FString& TxHash,
		const int64 ChainId,
		const FGetTransactionReceiptDelegate& SuccessDelegate,
		const FStringDelegate& ErrorDelegate
	);
}
