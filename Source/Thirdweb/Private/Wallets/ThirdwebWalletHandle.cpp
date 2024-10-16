// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "Wallets/ThirdwebWalletHandle.h"

#include "Thirdweb.h"
#include "ThirdwebCommon.h"
#include "ThirdwebInternal.h"
#include "ThirdwebMacros.h"
#include "ThirdwebRuntimeSettings.h"
#include "ThirdwebSigner.h"
#include "ThirdwebUtils.h"

#include "Dom/JsonObject.h"

#include "GenericPlatform/GenericPlatformHttp.h"

#include "Kismet/KismetStringLibrary.h"

#include "Misc/DefaultValueHelper.h"

#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

FString FWalletHandle::ToAddress() const
{
	FString Result;
	return Thirdweb::get_wallet_address(ID).AssignResult(Result) ? Result : ThirdwebUtils::ZeroAddress;
}

void FWalletHandle::Sign(const FString& Message, const FStringDelegate& SuccessDelegate, const FStringDelegate& ErrorDelegate) const
{
	CHECK_DELEGATES(SuccessDelegate, ErrorDelegate)
	CHECK_VALIDITY(ErrorDelegate)

	UE::Tasks::Launch(UE_SOURCE_LOCATION, [this, Message, SuccessDelegate, ErrorDelegate]
	{
		if (FString Error; Thirdweb::sign_message(ID, TO_RUST_STRING(Message)).AssignResult(Error))
		{
			SuccessDelegate.Execute(Error);
		} else
		{
			ErrorDelegate.Execute(Error);
		}
	});
}
