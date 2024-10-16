// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Wallets/InApp/Link/AsyncTaskThirdwebLinkOAuth.h"

#include "Components/SlateWrapperTypes.h"

void UAsyncTaskThirdwebLinkOAuth::Activate()
{
	InAppWallet.LinkOAuth(NewInAppWallet, AuthInput, BIND_UOBJECT_DELEGATE(FStreamableDelegate, HandleResponse), BIND_UOBJECT_DELEGATE(FStringDelegate, HandleFailed));
}
