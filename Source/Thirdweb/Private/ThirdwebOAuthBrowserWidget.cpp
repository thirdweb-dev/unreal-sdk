// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "ThirdwebOAuthBrowserWidget.h"

#include "ThirdwebLog.h"

#include "GenericPlatform/GenericPlatformHttp.h"

#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#if WITH_CEF
#include "SWebBrowser.h"
#endif

const FString UThirdwebOAuthBrowserWidget::DummyUrl = TEXT("about:blank");


bool UThirdwebOAuthBrowserWidget::IsPageLoaded() const
{
#if WITH_CEF
	return Browser.IsValid() && Browser->IsLoaded();
#else
	return false;
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
	if (!Browser.IsValid())
	{
		TW_LOG(Error, TEXT("OAuthBrowserWidget::Authenticate::Web browser invalid"));
		return;
	}
	TW_LOG(Log, TEXT("OAuthBrowserWidget::Authenticate::Loading %s"), *OAuthLoginUrl);
	Browser->LoadURL(OAuthLoginUrl);
#endif
}

void UThirdwebOAuthBrowserWidget::HandleUrlChanged(const FText& InUrl)
{
#if WITH_CEF
	OnUrlChanged.Broadcast(FGenericPlatformHttp::UrlDecode(InUrl.ToString()));
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
			.SupportsTransparency(bSupportsTransparency)
			.ShowInitialThrobber(bShowInitialThrobber)
			.OnUrlChanged(BIND_UOBJECT_DELEGATE(FOnTextChanged, HandleUrlChanged));

		return Browser.ToSharedRef();
	}
#endif
	return SNew(SBox).HAlign(HAlign_Center).VAlign(VAlign_Center)
		[
			SNew(STextBlock).Text(NSLOCTEXT("Thirdweb", "Thirdweb OAuth Browser", "Thirdweb OAuth Browser"))
		];
}


void UThirdwebOAuthBrowserWidget::OnWidgetRebuilt()
{
	Super::OnWidgetRebuilt();
}

