// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Wallets/InApp/SignIn/AsyncTaskThirdwebSignInWithAuthEndpoint.h"

#include "Components/SlateWrapperTypes.h"

void UAsyncTaskThirdwebSignInWithAuthEndpoint::Activate()
{
	InAppWallet.SignInWithAuthEndpoint(AuthInput, BIND_UOBJECT_DELEGATE(FStreamableDelegate, HandleResponse), BIND_UOBJECT_DELEGATE(FStringDelegate, HandleFailed));
}
