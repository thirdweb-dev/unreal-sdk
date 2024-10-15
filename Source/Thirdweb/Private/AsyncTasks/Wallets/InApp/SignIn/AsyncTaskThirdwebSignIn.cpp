// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Wallets/InApp/SignIn/AsyncTaskThirdwebSignIn.h"

#include "Components/SlateWrapperTypes.h"

void UAsyncTaskThirdwebSignIn::Activate()
{
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
	default: return HandleFailed(TEXT("Invalid Wallet"));
	}
}
