// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Wallets/InApp/Link/AsyncTaskThirdwebLinkGuest.h"

#include "Components/SlateWrapperTypes.h"

void UAsyncTaskThirdwebLinkGuest::Activate()
{
	InAppWallet.LinkGuest(NewInAppWallet, BIND_UOBJECT_DELEGATE(FStreamableDelegate, HandleResponse), BIND_UOBJECT_DELEGATE(FStringDelegate, HandleFailed));
}
