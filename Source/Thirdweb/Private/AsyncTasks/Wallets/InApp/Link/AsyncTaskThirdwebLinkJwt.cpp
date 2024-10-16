// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Wallets/InApp/Link/AsyncTaskThirdwebLinkJwt.h"

#include "Components/SlateWrapperTypes.h"

void UAsyncTaskThirdwebLinkJwt::Activate()
{
	InAppWallet.LinkJwt(NewInAppWallet, AuthInput, BIND_UOBJECT_DELEGATE(FStreamableDelegate, HandleResponse), BIND_UOBJECT_DELEGATE(FStringDelegate, HandleFailed));
}
