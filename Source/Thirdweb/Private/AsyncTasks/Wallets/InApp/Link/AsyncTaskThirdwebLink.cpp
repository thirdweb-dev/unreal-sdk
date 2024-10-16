// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Wallets/InApp/Link/AsyncTaskThirdwebLink.h"

#include "Components/SlateWrapperTypes.h"

void UAsyncTaskThirdwebLink::Activate()
{
	switch (InAppWallet.GetSource())
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
	default: return HandleFailed(TEXT("Invalid Wallet"));
	}
}
