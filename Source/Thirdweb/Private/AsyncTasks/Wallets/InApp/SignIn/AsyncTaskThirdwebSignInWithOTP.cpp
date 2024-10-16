// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Wallets/InApp/SignIn/AsyncTaskThirdwebSignInWithOTP.h"

#include "Components/SlateWrapperTypes.h"

void UAsyncTaskThirdwebSignInWithOTP::Activate()
{
	InAppWallet.SignInWithOTP(AuthInput, BIND_UOBJECT_DELEGATE(FStreamableDelegate, HandleResponse), BIND_UOBJECT_DELEGATE(FStringDelegate, HandleFailed));
}
