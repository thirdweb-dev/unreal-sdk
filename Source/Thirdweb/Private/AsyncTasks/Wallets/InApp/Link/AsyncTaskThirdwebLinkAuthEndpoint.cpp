// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Wallets/InApp/Link/AsyncTaskThirdwebLinkAuthEndpoint.h"

#include "Components/SlateWrapperTypes.h"

void UAsyncTaskThirdwebLinkAuthEndpoint::Activate()
{
	InAppWallet.LinkAuthEndpoint(NewInAppWallet, AuthInput, BIND_UOBJECT_DELEGATE(FStreamableDelegate, HandleResponse), BIND_UOBJECT_DELEGATE(FStringDelegate, HandleFailed));
}
