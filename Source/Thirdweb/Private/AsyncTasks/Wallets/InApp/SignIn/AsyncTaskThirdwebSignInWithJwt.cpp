// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Wallets/InApp/SignIn/AsyncTaskThirdwebSignInWithJwt.h"

#include "Components/SlateWrapperTypes.h"

void UAsyncTaskThirdwebSignInWithJwt::Activate()
{
	InAppWallet.SignInWithJwt(AuthInput, BIND_UOBJECT_DELEGATE(FStreamableDelegate, HandleResponse), BIND_UOBJECT_DELEGATE(FStringDelegate, HandleFailed));
}
