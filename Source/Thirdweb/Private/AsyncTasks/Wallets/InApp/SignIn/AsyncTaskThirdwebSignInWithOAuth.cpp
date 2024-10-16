// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Wallets/InApp/SignIn/AsyncTaskThirdwebSignInWithOAuth.h"

#include "Components/SlateWrapperTypes.h"

void UAsyncTaskThirdwebSignInWithOAuth::Activate()
{
	InAppWallet.SignInWithOAuth(AuthInput, BIND_UOBJECT_DELEGATE(FStreamableDelegate, HandleResponse), BIND_UOBJECT_DELEGATE(FStringDelegate, HandleFailed));
}
