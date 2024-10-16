// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Wallets/InApp/SignIn/AsyncTaskThirdwebSignInWithGuest.h"

#include "Components/SlateWrapperTypes.h"

void UAsyncTaskThirdwebSignInWithGuest::Activate()
{
	InAppWallet.SignInWithGuest(BIND_UOBJECT_DELEGATE(FStreamableDelegate, HandleResponse), BIND_UOBJECT_DELEGATE(FStringDelegate, HandleFailed));
}
