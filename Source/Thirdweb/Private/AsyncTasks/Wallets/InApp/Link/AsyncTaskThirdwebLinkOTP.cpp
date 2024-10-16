// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Wallets/InApp/Link/AsyncTaskThirdwebLinkOTP.h"

#include "Components/SlateWrapperTypes.h"

void UAsyncTaskThirdwebLinkOTP::Activate()
{
	InAppWallet.LinkOTP(NewInAppWallet, AuthInput, BIND_UOBJECT_DELEGATE(FStreamableDelegate, HandleResponse), BIND_UOBJECT_DELEGATE(FStringDelegate, HandleFailed));
}
