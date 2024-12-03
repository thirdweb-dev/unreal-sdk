// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "Engine/BackendWallet/ThirdwebEngine_BackendWallet.h"

#include "ThirdwebLog.h"
#include "ThirdwebUtils.h"
#include "Engine/ThirdwebEngine.h"
#include "Engine/BackendWallet/ThirdwebBackendWallet.h"
#include "Engine/Transaction/ThirdwebEngineTransactionOverrides.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Internal/ThirdwebHeaders.h"
#include "Internal/ThirdwebURLSearchParams.h"
#include "Kismet/KismetStringLibrary.h"
#include "Wallets/ThirdwebSmartWalletHandle.h"

namespace ThirdwebEngine::BackendWallet
{
	FString FormatUrl(const FString& Endpoint, const FThirdwebURLSearchParams& Params)
	{
		return ThirdwebEngine::FormatUrl(TEXT("backend-wallet"), Endpoint, Params);
	}

	extern void GetAll(
		const UObject* Outer,
		const int32 Page,
		const int32 Limit,
		const FGetAllDelegate& SuccessDelegate,
		const FStringDelegate& ErrorDelegate
	)
	{
		const TSharedRef<IHttpRequest> Request = ThirdwebUtils::Internal::CreateEngineRequest();
		Request->SetURL(FormatUrl(TEXT("get-all"), FThirdwebURLSearchParams::Create({{TEXT("page"), FString::FromInt(Page)}, {TEXT("limit"), FString::FromInt(Limit)}})));
		ThirdwebUtils::Internal::LogRequest(Request);
		Request->OnProcessRequestComplete().BindWeakLambda(Outer, [SuccessDelegate, ErrorDelegate](HTTP_LAMBDA_PARAMS)
		{
			CHECK_NETWORK
			FString Content = Response->GetContentAsString();
			TW_LOG(Verbose, TEXT("ThirdwebEngine::BackendWallet::GetAll::Content=%s"), *Content)
			FString Error;
			if (TArray<TSharedPtr<FJsonValue>> JsonArray; ThirdwebUtils::Json::ParseEngineResponse(Content, JsonArray, Error))
			{
				EXECUTE_IF_BOUND(SuccessDelegate, FThirdwebBackendWallet::FromJson(JsonArray))
			}
			else
			{
				EXECUTE_IF_BOUND(ErrorDelegate, Error)
			}
		});
		Request->ProcessRequest();
	}
}
