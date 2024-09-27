// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "ThirdwebRuntimeSettings.h"

#include "ThirdwebCommon.h"
#include "ThirdwebInternal.h"

UThirdwebRuntimeSettings::UThirdwebRuntimeSettings()
{
	AuthenticationMethod = EThirdwebAuthenticationMethod::ClientID;
	bSendAnalytics = true;
	bOverrideOAuthBrowserProviderBackends = false;
	OAuthBrowserProviderBackendOverrides[static_cast<int>(EThirdwebOAuthProvider::Google)] = EThirdwebOAuthBrowserBackend::External;
	OAuthBrowserProviderBackendOverrides[static_cast<int>(EThirdwebOAuthProvider::Telegram)] = EThirdwebOAuthBrowserBackend::External;
}

void UThirdwebRuntimeSettings::GenerateEncryptionKey()
{
#if WITH_EDITOR
	EncryptionKey = FThirdwebAnalytics::GenerateUUID();
	if (MarkPackageDirty())
	{

		PostEditChange();
	}
#endif
}
