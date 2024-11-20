// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "AsyncTasks/Wallets/InApp/SignIn/AsyncTaskThirdwebSignInWithEthereum.h"

#include "Components/SlateWrapperTypes.h"

void UAsyncTaskThirdwebSignInWithEthereum::Activate()
{
	UE_LOG(LogTemp, Warning, TEXT("Starting SignInWithEthereum! - Wallet:%s"), InAppWallet.GetSourceString());
	InAppWallet.SignInWithEthereum(AuthInput, SignatureInput, BIND_UOBJECT_DELEGATE(FStreamableDelegate, HandleResponse), BIND_UOBJECT_DELEGATE(FStringDelegate, HandleFailed));
}
