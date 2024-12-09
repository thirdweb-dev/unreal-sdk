// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Wallets/InApp/AsyncTaskThirdwebSignIn.h"

#include "Async/TaskGraphInterfaces.h"
#include "Components/SlateWrapperTypes.h"

UAsyncTaskThirdwebSignIn* UAsyncTaskThirdwebSignIn::SignIn(UObject* WorldContextObject, const FInAppWalletHandle& Wallet, const FString& Input, const FString& Signature)
{
	NEW_TASK
	Task->InAppWallet = Wallet;
	Task->AuthInput = Input;
	Task->SignatureInput = Signature;
	UE_LOG(LogTemp, Display, TEXT("UAsyncTaskThirdwebSignIn::Variables set"));
	RR_TASK
}

void UAsyncTaskThirdwebSignIn::Activate()
{
	
	UE_LOG(LogTemp, Display, TEXT("UAsyncTaskThirdwebSignIn::Activate::%s - %s - %lld"), InAppWallet.GetSourceString(), InAppWallet.GetTypeString(), InAppWallet.GetID());
	switch (InAppWallet.GetSource())
	{
	case FInAppWalletHandle::Email: return InAppWallet.SignInWithOTP(AuthInput, BIND_UOBJECT_DELEGATE(FStreamableDelegate, HandleResponse), BIND_UOBJECT_DELEGATE(FStringDelegate, HandleFailed));
	case FInAppWalletHandle::Phone: return InAppWallet.SignInWithOTP(AuthInput, BIND_UOBJECT_DELEGATE(FStreamableDelegate, HandleResponse), BIND_UOBJECT_DELEGATE(FStringDelegate, HandleFailed));
	case FInAppWalletHandle::OAuthProvider: return InAppWallet.SignInWithOAuth(AuthInput, BIND_UOBJECT_DELEGATE(FStreamableDelegate, HandleResponse),
	                                                                           BIND_UOBJECT_DELEGATE(FStringDelegate, HandleFailed));
	case FInAppWalletHandle::Jwt: return InAppWallet.SignInWithJwt(AuthInput, BIND_UOBJECT_DELEGATE(FStreamableDelegate, HandleResponse), BIND_UOBJECT_DELEGATE(FStringDelegate, HandleFailed));
	case FInAppWalletHandle::Guest: return InAppWallet.SignInWithGuest(BIND_UOBJECT_DELEGATE(FStreamableDelegate, HandleResponse), BIND_UOBJECT_DELEGATE(FStringDelegate, HandleFailed));
	case FInAppWalletHandle::AuthEndpoint: return InAppWallet.SignInWithAuthEndpoint(AuthInput, BIND_UOBJECT_DELEGATE(FStreamableDelegate, HandleResponse),
	                                                                                 BIND_UOBJECT_DELEGATE(FStringDelegate, HandleFailed));
	case FInAppWalletHandle::Siwe: return InAppWallet.SignInWithEthereum(AuthInput, SignatureInput, BIND_UOBJECT_DELEGATE(FStreamableDelegate, HandleResponse),
	                                                                     BIND_UOBJECT_DELEGATE(FStringDelegate, HandleFailed));
	default: return HandleFailed(TEXT("Invalid Wallet"));
	}
}

void UAsyncTaskThirdwebSignIn::HandleResponse()
{
	if (IsInGameThread())
	{
		Success.Broadcast(TEXT(""));
		return SetReadyToDestroy();
	}

	// Retry on the GameThread.
	TWeakObjectPtr<UAsyncTaskThirdwebSignIn> WeakThis = this;
	FFunctionGraphTask::CreateAndDispatchWhenReady([WeakThis]()
	{
		if (WeakThis.IsValid())
		{
			WeakThis->HandleResponse();
		}
	}, TStatId(), nullptr, ENamedThreads::GameThread);
}

void UAsyncTaskThirdwebSignIn::HandleFailed(const FString& Error)
{
	if (IsInGameThread())
	{
		Failed.Broadcast(Error);
		return SetReadyToDestroy();
	}

	TWeakObjectPtr<UAsyncTaskThirdwebSignIn> WeakThis = this;
	FFunctionGraphTask::CreateAndDispatchWhenReady([WeakThis, Error]()
	{
		if (WeakThis.IsValid())
		{
			WeakThis->HandleFailed(Error);
		}
	}, TStatId(), nullptr, ENamedThreads::GameThread);
}