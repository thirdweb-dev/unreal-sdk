// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "Browser/ThirdwebOAuthBrowserUserWidget.h"

#include "ThirdwebLog.h"

#include "Blueprint/WidgetTree.h"

#include "Browser/ThirdwebOAuthBrowserWidget.h"

#include "Components/Button.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"
#include "Components/PanelWidget.h"

const FString UThirdwebOAuthBrowserUserWidget::BackendUrlPrefix = TEXT("https://embedded-wallet.thirdweb.com/");

TSharedRef<SWidget> UThirdwebOAuthBrowserUserWidget::RebuildWidget()
{
	// RebuildWidget is not called until the widget is first added to the
	// viewport.

	UPanelWidget* RootWidget = Cast<UPanelWidget>(GetRootWidget());

	// Construct root widget if needed
	if (!RootWidget)
	{
		RootWidget = WidgetTree->ConstructWidget<UOverlay>(UOverlay::StaticClass(), TEXT("RootWidget"));
		WidgetTree->RootWidget = RootWidget;
	}

	// Construct children
	if (RootWidget)
	{
		// Construct browser widget
		Browser = WidgetTree->ConstructWidget<UThirdwebOAuthBrowserWidget>(UThirdwebOAuthBrowserWidget::StaticClass(), TEXT("ThirdwebOauthBrowser"));
		Browser->OnUrlChanged.AddUObject(this, &ThisClass::HandleUrlChanged);
		Browser->OnPageLoaded.AddUObject(this, &ThisClass::HandlePageLoaded);
		Browser->OnBeforePopup.AddUObject(this, &ThisClass::HandleOnBeforePopup);
		UPanelSlot* PanelSlot = RootWidget->AddChild(Browser);
		if (UOverlaySlot* RootWidgetSlot = Cast<UOverlaySlot>(PanelSlot))
		{
			RootWidgetSlot->SetHorizontalAlignment(HAlign_Fill);
			RootWidgetSlot->SetVerticalAlignment(VAlign_Fill);
		}
	}

	return Super::RebuildWidget();
}

#if WITH_EDITOR
const FText UThirdwebOAuthBrowserUserWidget::GetPaletteCategory()
{
	return NSLOCTEXT("Thirdweb", "Thirdweb", "Thirdweb");
}
#endif

void UThirdwebOAuthBrowserUserWidget::OnWidgetRebuilt()
{
	Super::OnWidgetRebuilt();
	SetVisible(false);
}

void UThirdwebOAuthBrowserUserWidget::Authenticate(const FInAppWalletHandle& InAppWallet)
{
	if (!InAppWallet.IsValid())
	{
		TW_LOG(Error, TEXT("OAuthBrowserUserWidget::Authenticate::Wallet invalid"));
		return OnError.Broadcast(TEXT("Invalid Wallet"));
	}
	Wallet = InAppWallet;

	if (Browser)
	{
		FString Error;
		if (FString Link; Wallet.FetchOAuthLoginURL(Browser->DummyUrl, Link, Error))
		{
			TW_LOG(Verbose, TEXT("OAuthBrowserUserWidget::Authenticate::Authenticating against %s"), *Link);
			return Browser->Authenticate(Link);
		}
		OnError.Broadcast(Error);
	}
}


bool UThirdwebOAuthBrowserUserWidget::IsBlank() const
{
	FString Url = Browser->GetUrl();

	return Url.IsEmpty() || Url.StartsWith(BackendUrlPrefix);
}

FString UThirdwebOAuthBrowserUserWidget::GetUrl() const
{
	if (Browser)
	{
		return Browser->GetUrl();
	}
	return TEXT("");
}

void UThirdwebOAuthBrowserUserWidget::HandleUrlChanged(const FString& Url)
{
	TW_LOG(Verbose, TEXT("OAuthBrowserUserWidget::HandleUrlChanged::%s"), *Url);
	if (Url.IsEmpty() || Url.StartsWith(BackendUrlPrefix))
	{
		return SetVisible(false);
	}
	if (Url.StartsWith(Browser->DummyUrl))
	{
		SetVisible(false);
		FString Left, Right;
		if (Url.Split(TEXT("authResult="), &Left, &Right, ESearchCase::IgnoreCase))
		{
			return OnAuthenticated.Broadcast(Right);
		}
		return OnError.Broadcast(TEXT("Failed to match AuthResult in url"));
	}
	bShouldBeVisible = true;
}

void UThirdwebOAuthBrowserUserWidget::HandlePageLoaded(const FString& Url)
{
	if (bShouldBeVisible)
	{
		SetVisible(true);
	}
}

void UThirdwebOAuthBrowserUserWidget::HandleOnBeforePopup(const FString& Url, const FString& Frame)
{
	return OnPopup.Broadcast(Url, Frame);
}

void UThirdwebOAuthBrowserUserWidget::SetVisible(const bool bVisible)
{
	// Mobile webview needs to be visible to work
	if (bVisible)
	{
		if (bCollapseWhenBlank)
		{
#if PLATFORM_IOS | PLATFORM_ANDROID
			SetRenderOpacity(1.0f);
#else
			SetVisibility(ESlateVisibility::Visible);
#endif
		}
	}
	else
	{
		bShouldBeVisible = false;
		if (bCollapseWhenBlank)
		{
#if PLATFORM_IOS | PLATFORM_ANDROID
			SetRenderOpacity(0.01f);
#else
			SetVisibility(ESlateVisibility::Collapsed);
#endif
		}
	}
}
