// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Wallets/InApp/AsyncTaskThirdwebLink.h"

#include "Components/SlateWrapperTypes.h"

UAsyncTaskThirdwebLink* UAsyncTaskThirdwebLink::Link(UObject* WorldContextObject, const FInAppWalletHandle& Wallet, const FInAppWalletHandle& NewWallet, const FString& Input, const FString& Signature)
{
	NEW_TASK
	Task->InAppWallet = Wallet;
	Task->NewInAppWallet = NewWallet;
	Task->AuthInput = Input;
	Task->SignatureInput = Signature;
	RR_TASK
}

void UAsyncTaskThirdwebLink::Activate()
{
	switch (NewInAppWallet.GetSource())
	{
	case FInAppWalletHandle::Email: return InAppWallet.LinkOTP(
			NewInAppWallet,
			AuthInput,
			BIND_UOBJECT_DELEGATE(FStreamableDelegate, HandleResponse),
			BIND_UOBJECT_DELEGATE(FStringDelegate, HandleFailed)
		);
	case FInAppWalletHandle::Phone: return InAppWallet.LinkOTP(
			NewInAppWallet,
			AuthInput,
			BIND_UOBJECT_DELEGATE(FStreamableDelegate, HandleResponse),
			BIND_UOBJECT_DELEGATE(FStringDelegate, HandleFailed)
		);
	case FInAppWalletHandle::OAuthProvider: return InAppWallet.LinkOAuth(
			NewInAppWallet,
			AuthInput,
			BIND_UOBJECT_DELEGATE(FStreamableDelegate, HandleResponse),
			BIND_UOBJECT_DELEGATE(FStringDelegate, HandleFailed)
		);
	case FInAppWalletHandle::Jwt: return InAppWallet.LinkJwt(
			NewInAppWallet,
			AuthInput,
			BIND_UOBJECT_DELEGATE(FStreamableDelegate, HandleResponse),
			BIND_UOBJECT_DELEGATE(FStringDelegate, HandleFailed)
		);
	case FInAppWalletHandle::Guest: return InAppWallet.LinkGuest(
			NewInAppWallet,
			BIND_UOBJECT_DELEGATE(FStreamableDelegate, HandleResponse),
			BIND_UOBJECT_DELEGATE(FStringDelegate, HandleFailed)
		);
	case FInAppWalletHandle::AuthEndpoint: return InAppWallet.LinkAuthEndpoint(
			NewInAppWallet,
			AuthInput,
			BIND_UOBJECT_DELEGATE(FStreamableDelegate, HandleResponse),
			BIND_UOBJECT_DELEGATE(FStringDelegate, HandleFailed)
		);
	case FInAppWalletHandle::Siwe: return InAppWallet.LinkSiwe(
			NewInAppWallet,
			AuthInput,
			SignatureInput,
			BIND_UOBJECT_DELEGATE(FStreamableDelegate, HandleResponse),
			BIND_UOBJECT_DELEGATE(FStringDelegate, HandleFailed)
		);
	default: return HandleFailed(TEXT("Invalid Wallet"));
	}
}

void UAsyncTaskThirdwebLink::HandleResponse()
{
	if (IsInGameThread())
	{
		Success.Broadcast(TEXT(""));
		return SetReadyToDestroy();
	}

	TWeakObjectPtr<UAsyncTaskThirdwebLink> WeakThis = this;
	FFunctionGraphTask::CreateAndDispatchWhenReady([WeakThis]()
	{
		if (WeakThis.IsValid())
		{
			WeakThis->HandleResponse();
		}
	}, TStatId(), nullptr, ENamedThreads::GameThread);
}

void UAsyncTaskThirdwebLink::HandleFailed(const FString& Error)
{
	if (IsInGameThread())
	{
		Failed.Broadcast(Error);
		return SetReadyToDestroy();
	}

	TWeakObjectPtr<UAsyncTaskThirdwebLink> WeakThis = this;
	FFunctionGraphTask::CreateAndDispatchWhenReady([WeakThis, Error]()
	{
		if (WeakThis.IsValid())
		{
			WeakThis->HandleFailed(Error);
		}
	}, TStatId(), nullptr, ENamedThreads::GameThread);
}
