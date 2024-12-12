// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "Browser/ThirdwebOAuthBrowserUserWidget.h"

#include "ThirdwebLog.h"
#include "ThirdwebRuntimeSettings.h"
#include "Blueprint/WidgetTree.h"
#include "Browser/ThirdwebOAuthExternalBrowser.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"
#include "Components/PanelWidget.h"

#if PLATFORM_ANDROID
#include "Android/AndroidApplication.h"
#include "Browser/Android/ThirdwebAndroidJNI.h"
#endif

const FString UThirdwebOAuthBrowserUserWidget::BackendUrlPrefix = TEXT("https://embedded-wallet.thirdweb.com/");
const FString UThirdwebOAuthBrowserUserWidget::DummyUrl = TEXT("http://localhost:8789/callback");

TSharedRef<SWidget> UThirdwebOAuthBrowserUserWidget::RebuildWidget()
{
	// RebuildWidget is not called until the widget is first added to the
	// viewport.

	UPanelWidget *RootWidget = Cast<UPanelWidget>(GetRootWidget());

	// Construct root widget if needed
	if (!RootWidget)
	{
		RootWidget = WidgetTree->ConstructWidget<UOverlay>(UOverlay::StaticClass(), TEXT("RootWidget"));
		WidgetTree->RootWidget = RootWidget;
	}

	// Construct children
	if (RootWidget)
	{
		// Construct External browser
		ExternalBrowser = NewObject<UThirdwebOAuthExternalBrowser>(this);
		ExternalBrowser->OnAuthenticated.BindUObject(this, &ThisClass::HandleAuthenticated);
		ExternalBrowser->OnError.BindUObject(this, &ThisClass::HandleError);
		ExternalBrowser->OnSiweComplete.BindUObject(this, &ThisClass::HandleSiweComplete);
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

void UThirdwebOAuthBrowserUserWidget::BeginDestroy()
{
	if (ExternalBrowser)
	{
		ExternalBrowser->ConditionalBeginDestroy();
	}
	Super::BeginDestroy();
}

FString UThirdwebOAuthBrowserUserWidget::GetDummyUrl()
{
#if PLATFORM_ANDROID
	return UThirdwebRuntimeSettings::GetAppUri();
#else
	return DummyUrl;
#endif
}

void UThirdwebOAuthBrowserUserWidget::Authenticate(const FInAppWalletHandle &InAppWallet)
{
	// Validate Wallet
	if (!InAppWallet.IsValid())
	{
		TW_LOG(Error, TEXT("OAuthBrowserUserWidget::Authenticate::Wallet invalid"));
		return HandleError(TEXT("Invalid Wallet"));
	}
	Wallet = InAppWallet;
	TW_LOG(VeryVerbose, TEXT("OAuthBrowserUserWidget::Authenticate::Wallet Type::%s"), Wallet.GetSourceString());
	if (Wallet == FInAppWalletHandle::Siwe)
	{
		TW_LOG(VeryVerbose, TEXT("OAuthBrowserUserWidget::Authenticate::Authenticating against SIWE"));
		ExternalBrowser->Authenticate(TEXT("SIWE"));
		return;
	}

	// Get Login URL
	FString Link;
	if (FString Error; !Wallet.FetchOAuthLoginURL(GetDummyUrl(), Link, Error))
	{
		return HandleError(Error);
	}
	TW_LOG(VeryVerbose, TEXT("OAuthBrowserUserWidget::Authenticate::Authenticating against %s"), *Link);

#if PLATFORM_ANDROID
	if (JNIEnv *Env = FAndroidApplication::GetJavaEnv())
	{
		jstring JUrl = Env->NewStringUTF(TCHAR_TO_UTF8(*Link));
		jclass JClass = FAndroidApplication::FindJavaClass("com/thirdweb/unrealengine/ThirdwebActivity");
		static jmethodID JLaunchUrl = FJavaWrapper::FindStaticMethod(Env, JClass, "startActivity", "(Landroid/app/Activity;Ljava/lang/String;)V", false);
		ThirdwebUtils::Internal::Android::CallJniStaticVoidMethod(Env, JClass, JLaunchUrl, FJavaWrapper::GameActivityThis, JUrl);
		TW_LOG(Verbose, TEXT("OAuthBrowserUserWidget::Authenticate::Opening CustomTabs"));
		return;
	}
	TW_LOG(Error, TEXT("OAuthBrowserUserWidget::Authenticate::No JNIEnv found"));
	return;
#endif

	return ExternalBrowser->Authenticate(Link);
}

void UThirdwebOAuthBrowserUserWidget::HandleUrlChanged(const FString &Url)
{
	TW_LOG(Verbose, TEXT("OAuthBrowserUserWidget::HandleUrlChanged::%s"), *Url);
	if (Url.IsEmpty() || (Url.StartsWith(BackendUrlPrefix) && !Url.StartsWith(BackendUrlPrefix + TEXT("sdk/oauth"))))
	{
		return SetVisible(false);
	}
	if (Url.StartsWith(GetDummyUrl()))
	{
		SetVisible(false);
		FString Left, Right;
		if (Url.Split(TEXT("authResult="), &Left, &Right, ESearchCase::IgnoreCase))
		{
			return HandleAuthenticated(Right);
		}
		return HandleError(TEXT("Failed to match AuthResult in url"));
	}
	bShouldBeVisible = true;
}

void UThirdwebOAuthBrowserUserWidget::HandlePageLoaded(const FString &Url)
{
	if (bShouldBeVisible)
	{
		SetVisible(true);
	}
}

void UThirdwebOAuthBrowserUserWidget::HandleOnBeforePopup(const FString &Url, const FString &Frame)
{
	return OnPopup.Broadcast(Url, Frame);
}

void UThirdwebOAuthBrowserUserWidget::HandleAuthenticated(const FString &AuthResult)
{
	OnAuthenticated.Broadcast(AuthResult);
}

void UThirdwebOAuthBrowserUserWidget::HandleSiweComplete(const FString &Signature, const FString &Payload)
{
	OnSiweComplete.Broadcast(Signature, Payload);
}

void UThirdwebOAuthBrowserUserWidget::HandleError(const FString &Error)
{
	OnError.Broadcast(Error);
}

#if PLATFORM_ANDROID
void UThirdwebOAuthBrowserUserWidget::HandleDeepLink(const FString &Url)
{
	TW_LOG(VeryVerbose, TEXT("UThirdwebOAuthBrowserUserWidget::HandleDeepLink::%s"), *Url);
	HandleUrlChanged(Url);
}

void UThirdwebOAuthBrowserUserWidget::HandleCustomTabsDismissed(const FString &Url)
{
	TW_LOG(VeryVerbose, TEXT("UThirdwebOAuthBrowserUserWidget::HandleCustomTabsDismissed::%s"), *Url);
	HandleUrlChanged(Url);
}
#endif

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
