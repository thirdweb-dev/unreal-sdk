// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "Engine/Marketplace/ThirdwebEngine_Marketplace.h"

#include "ThirdwebRuntimeSettings.h"
#include "Engine/ThirdwebAccountIdentifierParams.h"
#include "Engine/ThirdwebEngine.h"
#include "Internal/ThirdwebHeaders.h"
#include "Internal/ThirdwebURLSearchParams.h"
#include "Wallets/ThirdwebSmartWalletHandle.h"

namespace ThirdwebEngine::Marketplace
{
	FString FormatUrl(const int64 ChainId, const FString& ContractAddress, const FString& Section, const FString& Endpoint, const FThirdwebURLSearchParams& Params)
	{
		return ThirdwebEngine::FormatUrl(TEXT("marketplace"), FString::Printf(TEXT("%lld/%s/%s/%s"), ChainId, *ContractAddress, *Section, *Endpoint), Params);
	}

	FThirdwebHeaders MakePostHeaders(const FString& BackendWalletAddress, const FString& IdempotencyKey, const FThirdwebAccountIdentifierParams& Account)
	{
		FThirdwebHeaders Headers;
		Headers.Set(TEXT("Content-Type"), TEXT("application/json"));
		Headers.Set(TEXT("x-backend-wallet-address"), BackendWalletAddress);
		Headers.Set(TEXT("x-idempotency-key"), IdempotencyKey);
		Headers.Set(TEXT("x-account-address"), Account.SmartWallet.ToAddress(), Account.SmartWallet.IsValid());
		Headers.Set(TEXT("x-account-factory-address"), Account.FactoryAddress);
		Headers.Set(TEXT("x-account-salt"), Account.Salt);
		return Headers;
	}
}
