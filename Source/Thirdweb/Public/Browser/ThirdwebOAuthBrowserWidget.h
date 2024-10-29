// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#if WITH_CEF
#include "SWebBrowser.h"
#endif
#include "Components/Widget.h"

#include "Wallets/ThirdwebInAppWalletHandle.h"

#include "ThirdwebOAuthBrowserWidget.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FSimpleStringDelegate, const FString&);
DECLARE_MULTICAST_DELEGATE_TwoParams(FSimpleDoubleStringDelegate, const FString&, const FString&);
DECLARE_DELEGATE_RetVal_ThreeParams(bool, FOnLoadUrl, const FString& /*Method*/, const FString& /*Url*/, FString& /* Response */)

UCLASS(NotBlueprintable, NotBlueprintType, Hidden)
class THIRDWEB_API UThirdwebOAuthBrowserWidget : public UWidget
{
	GENERATED_BODY()

public:
	//~ Begin Public Overrides
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override;
#endif
	//~ End Public Overrides

	virtual void HandleUrlChanged(const FText& InUrl);
	virtual void HandleOnLoadComplete();
	virtual bool HandleOnBeforePopup(FString Url, FString Frame);
	virtual bool HandleOnCreateWindow(const TWeakPtr<IWebBrowserWindow>& Window, const TWeakPtr<IWebBrowserPopupFeatures>& Features);
	virtual void HandleOnLoadError();
	virtual bool HandleOnCloseWindow(const TWeakPtr<IWebBrowserWindow>& Window);
	virtual void LoadUrl(const FString& Url);
	
	bool IsPageLoaded() const;
	FString GetUrl() const;
	static FString GetDummyUrl();
	
	UFUNCTION(BlueprintCallable, Category="Thirdweb|OAuth Browser")
	void Authenticate(const FString& OAuthLoginUrl);

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void OnWidgetRebuilt() override;
	
public:
	FSimpleStringDelegate OnUrlChanged;
	FSimpleStringDelegate OnPageLoaded;
	FSimpleDoubleStringDelegate OnBeforePopup;
	
	

private:
	FString InitialUrl;
	bool bSupportsTransparency = false;
	bool bShowInitialThrobber = false;
	static const FString DummyUrl;
	
#if WITH_CEF
	TSharedPtr<SWebBrowser> Browser;
#endif
};
