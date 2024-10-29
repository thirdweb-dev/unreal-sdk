// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "Browser/ThirdwebOAuthBrowserWidget.h"

#include "ThirdwebLog.h"
#include "ThirdwebRuntimeSettings.h"

#include "Async/Async.h"

#include "GenericPlatform/GenericPlatformHttp.h"

#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#if WITH_CEF
#include "SWebBrowser.h"
#endif

#define ENSURE_VALID_BROWSER(FunctionName) \
	if (!Browser.IsValid()) \
	{ \
		TW_LOG(Error, TEXT("OAuthBrowserWidget::%s::Web browser invalid"), TEXT(FunctionName)); \
		return; \
	}

const FString UThirdwebOAuthBrowserWidget::DummyUrl = TEXT("http://localhost:8789/callback");

bool UThirdwebOAuthBrowserWidget::IsPageLoaded() const
{
#if WITH_CEF
	return Browser.IsValid() && Browser->IsLoaded();
#else
	return false;
#endif
}

FString UThirdwebOAuthBrowserWidget::GetUrl() const
{
#if WITH_CEF
	if (Browser.IsValid())
	{
		if (FString Url = Browser->GetUrl(); !Url.IsEmpty())
		{
			return Url;
		}
	}
#endif
	return TEXT("");
}

FString UThirdwebOAuthBrowserWidget::GetDummyUrl()
{
#if PLATFORM_ANDROID | PLATFORM_IOS
	return UThirdwebRuntimeSettings::GetAppUri();
#else
	return DummyUrl;
#endif
}

void UThirdwebOAuthBrowserWidget::ReleaseSlateResources(const bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
#if WITH_CEF
	Browser.Reset();
#endif
}

#if WITH_EDITOR
const FText UThirdwebOAuthBrowserWidget::GetPaletteCategory()
{
	return NSLOCTEXT("Thirdweb", "Thirdweb", "Thirdweb");
}
#endif

// ReSharper disable once CppUE4BlueprintCallableFunctionMayBeConst
void UThirdwebOAuthBrowserWidget::Authenticate(const FString& OAuthLoginUrl)
{
#if WITH_CEF
	ENSURE_VALID_BROWSER("Authenticate")
	TW_LOG(VeryVerbose, TEXT("OAuthBrowserWidget::Authenticate::Loading %s"), *OAuthLoginUrl);
	Browser->LoadURL(OAuthLoginUrl);
#endif
}

void UThirdwebOAuthBrowserWidget::HandleUrlChanged(const FText& InUrl)
{
#if WITH_CEF
	ENSURE_VALID_BROWSER("HandleUrlChanged")
	if (IsInGameThread())
	{
		FString Url = InUrl.ToString();
		if (Url.IsEmpty())
		{
			Url = Browser->GetUrl();
		}
		TW_LOG(VeryVerbose, TEXT("ThirdwebOAuthBrowserWidget::HandleUrlChanged:%s"), *Url);
		OnUrlChanged.Broadcast(FGenericPlatformHttp::UrlDecode(Url));
	}
	else
	{
		// Retry on the GameThread.
		TWeakObjectPtr<UThirdwebOAuthBrowserWidget> WeakThis = this;
		FFunctionGraphTask::CreateAndDispatchWhenReady([WeakThis, InUrl]()
		{
			if (WeakThis.IsValid())
			{
				WeakThis->HandleUrlChanged(InUrl);
			}
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	}

#endif
}

void UThirdwebOAuthBrowserWidget::HandleOnLoadComplete()
{
#if WITH_CEF
	ENSURE_VALID_BROWSER("HandleOnLoadComplete")
	if (IsInGameThread())
	{
		TW_LOG(VeryVerbose, TEXT("ThirdwebOAuthBrowserWidget::HandleOnLoadComplete:%s"), *Browser->GetUrl());
		OnPageLoaded.Broadcast(Browser->GetUrl());
	}
	else
	{
		// Retry on the GameThread.
		TWeakObjectPtr<UThirdwebOAuthBrowserWidget> WeakThis = this;
		FFunctionGraphTask::CreateAndDispatchWhenReady([WeakThis]()
		{
			if (WeakThis.IsValid())
			{
				WeakThis->HandleOnLoadComplete();
			}
		}, TStatId(), nullptr, ENamedThreads::GameThread);
	}
#endif
}

bool UThirdwebOAuthBrowserWidget::HandleOnBeforePopup(FString URL, FString Frame)
{
	TW_LOG(VeryVerbose, TEXT("ThirdwebOAuthBrowserWidget::HandleOnBeforePopup::%s | %s"), *URL, *Frame)
	if (OnBeforePopup.IsBound())
	{
		if (IsInGameThread())
		{
			OnBeforePopup.Broadcast(URL, Frame);
		}
		else
		{
			// Retry on the GameThread.
			TWeakObjectPtr<UThirdwebOAuthBrowserWidget> WeakThis = this;
			FFunctionGraphTask::CreateAndDispatchWhenReady([WeakThis, URL, Frame]()
			{
				if (WeakThis.IsValid())
				{
					WeakThis->HandleOnBeforePopup(URL, Frame);
				}
			}, TStatId(), nullptr, ENamedThreads::GameThread);
		}
		return true;
	}
	return false;
}

bool UThirdwebOAuthBrowserWidget::HandleOnCreateWindow(const TWeakPtr<IWebBrowserWindow>& Window, const TWeakPtr<IWebBrowserPopupFeatures>& Features)
{
	TW_LOG(Error, TEXT("ThirdwebOAuthBrowserWidget::HandleOnLoadError"))
	return false;
}

void UThirdwebOAuthBrowserWidget::HandleOnLoadError()
{
	TW_LOG(Error, TEXT("ThirdwebOAuthBrowserWidget::HandleOnLoadError"))
}

bool UThirdwebOAuthBrowserWidget::HandleOnCloseWindow(const TWeakPtr<IWebBrowserWindow>& Window)
{
	TW_LOG(VeryVerbose, TEXT("ThirdwebOAuthBrowserWidget::HandleOnCloseWindow"))
	RemoveFromParent();
	return true;
}

void UThirdwebOAuthBrowserWidget::LoadUrl(const FString& Url)
{
#if WITH_CEF
	ENSURE_VALID_BROWSER("LoadUrl")
	Browser->LoadURL(Url);
#endif
}

TSharedRef<SWidget> UThirdwebOAuthBrowserWidget::RebuildWidget()
{
#if WITH_CEF
	if (!IsDesignTime())
	{
		Browser = SNew(SWebBrowser)
		                           .InitialURL(InitialUrl)
		                           .ShowControls(false)
		                           .ShowAddressBar(false)
			// .PopupMenuMethod(EPopupMethod::UseCurrentWindow)
		                           .SupportsTransparency(bSupportsTransparency)
		                           .ShowInitialThrobber(bShowInitialThrobber)
		                           .OnCreateWindow(BIND_UOBJECT_DELEGATE(FOnCreateWindowDelegate, HandleOnCreateWindow))
		                           .OnLoadError(BIND_UOBJECT_DELEGATE(FSimpleDelegate, HandleOnLoadError))
		                           .OnUrlChanged(BIND_UOBJECT_DELEGATE(FOnTextChanged, HandleUrlChanged))
		                           .OnLoadCompleted(BIND_UOBJECT_DELEGATE(FSimpleDelegate, HandleOnLoadComplete))
		                           .OnCloseWindow(BIND_UOBJECT_DELEGATE(FOnCloseWindowDelegate, HandleOnCloseWindow))
		                           .OnBeforePopup(BIND_UOBJECT_DELEGATE(FOnBeforePopupDelegate, HandleOnBeforePopup));

		return Browser.ToSharedRef();
	}
#endif
	return SNew(SBox).HAlign(HAlign_Fill).VAlign(VAlign_Fill)
		[
			SNew(STextBlock).Text(NSLOCTEXT("Thirdweb", "Thirdweb OAuth Browser", "Thirdweb OAuth Browser"))
		];
}

void UThirdwebOAuthBrowserWidget::OnWidgetRebuilt()
{
	Super::OnWidgetRebuilt();
}
