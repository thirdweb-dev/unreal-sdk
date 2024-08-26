// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "ThirdwebOAuthBrowser.h"
#include "SWebBrowser.h"
#include "ThirdwebLog.h"
#include "WebBrowser.h"
#include "Blueprint/WidgetTree.h"
#include "GenericPlatform/GenericPlatformHttp.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"

#define LOCTEXT_NAMESPACE "Thirdweb"

void UThirdwebOAuthBrowser::Authenticate(const FWalletHandle& InAppWallet)
{
	Wallet = InAppWallet;
	FString Error;
	if (FString Link; Wallet.FetchOAuthLoginURL(TEXT("http://localhost:7879/callback"), Link, Error))
	{
		Browser->LoadURL(Link);
	} else
	{
		OnError.Broadcast(Error);
	}
}

bool UThirdwebOAuthBrowser::Initialize()
{
	const bool bInitializedThisCall = Super::Initialize();
	if (bInitializedThisCall)
	{
		if (!Browser)
		{
			Browser = WidgetTree->ConstructWidget<UWebBrowser>(UWebBrowser::StaticClass(), TEXT("Browser"));
		}
		WidgetTree->RootWidget = Browser;
		Browser->OnUrlChanged.AddDynamic(this, &ThisClass::HandleOnUrlChanged);
	}
	return bInitializedThisCall;
}

void UThirdwebOAuthBrowser::HandleOnUrlChanged(const FText& InURL)
{
	if (FString URL = InURL.ToString(); URL.StartsWith(TEXT("https://embedded-wallet.thirdweb.com/")))
	{
		Browser->SetVisibility(ESlateVisibility::Hidden);
	} else if (URL.StartsWith(TEXT("http://localhost:7879/callback")))
	{
		Browser->SetVisibility(ESlateVisibility::Hidden);
		FString Left, Right;
		if (URL.Split(TEXT("authResult="), &Left, &Right, ESearchCase::IgnoreCase))
		{
			if (FString Error; Wallet.SignInWithOAuth(FGenericPlatformHttp::UrlDecode(Right), Error))
			{
				OnSuccess.Broadcast();
			} else
			{
				OnError.Broadcast(Error);
			}
		} else
		{
			OnError.Broadcast(TEXT("Failed to get AuthResult"));
		}
	} else
	{
		Browser->SetVisibility(ESlateVisibility::Visible);
	}
}


#if WITH_EDITOR
const FText UThirdwebOAuthBrowser::GetPaletteCategory()
{
	return LOCTEXT("Thirdweb", "Thirdweb");
}
#endif

#undef LOCTEXT_NAMESPACE
