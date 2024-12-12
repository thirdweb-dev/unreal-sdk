// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "Blueprint/UserWidget.h"
#include "Wallets/ThirdwebInAppWalletHandle.h"
#include "ThirdwebOAuthBrowserUserWidget.generated.h"

UCLASS(DisplayName = "OAuth Browser")
class THIRDWEB_API UThirdwebOAuthBrowserUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAuthenticatedDelegate, const FString &, AuthResult);
	UPROPERTY(BlueprintAssignable, Category = "Thirdweb|OAuth Browser")
	FOnAuthenticatedDelegate OnAuthenticated;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSiweCompleteDelegate, const FString &, Signature, const FString &, Payload);
	UPROPERTY(BlueprintAssignable, Category = "Thirdweb|OAuth Browser")
	FOnSiweCompleteDelegate OnSiweComplete;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnErrorDelegate, const FString &, Error);
	UPROPERTY(BlueprintAssignable, Category = "Thirdweb|OAuth Browser")
	FOnErrorDelegate OnError;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUrlDelegate, const FString &, URL);
	UPROPERTY(BlueprintAssignable, DisplayName = "On URL Changed", Category = "Thirdweb|OAuth Browser")
	FUrlDelegate OnUrlChanged;

	UPROPERTY(BlueprintAssignable, Category = "Thirdweb|OAuth Browser")
	FUrlDelegate OnPageLoaded;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPopupDelegate, const FString &, URL, const FString &, Frame);
	UPROPERTY(BlueprintAssignable, Category = "Thirdweb|OAuth Browser")
	FPopupDelegate OnPopup;

protected:
	/** Automatically collapse the widget when the page is blank */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Browser")
	bool bCollapseWhenBlank = true;

	/** Automatically authenticate on Construct. Only works when created with a Wallet */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Browser")
	bool bAuthenticateOnConstruct = false;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "Internal", meta = (ExposeOnSpawn = true))
	FInAppWalletHandle Wallet;

private:
	UPROPERTY(Transient)
	class UThirdwebOAuthExternalBrowser *ExternalBrowser = nullptr;

	static const FString BackendUrlPrefix;
	static const FString DummyUrl;
	bool bShouldBeVisible = false;

public:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void OnWidgetRebuilt() override;
	virtual void BeginDestroy() override;

#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override;
#endif

protected:
	void SetVisible(const bool bVisible);
	virtual FString GetDummyUrl();
	virtual void HandleUrlChanged(const FString &Url);
	virtual void HandlePageLoaded(const FString &Url);
	virtual void HandleOnBeforePopup(const FString &Url, const FString &Frame);

	virtual void HandleAuthenticated(const FString &AuthResult);
	virtual void HandleSiweComplete(const FString &Signature, const FString &Payload);
	virtual void HandleError(const FString &Error);

public:
#if PLATFORM_ANDROID
	void HandleDeepLink(const FString &Url);
	void HandleCustomTabsDismissed(const FString &Url);
#endif

	UFUNCTION(BlueprintCallable, Category = "Thirdweb|OAuth Browser")
	void Authenticate(const FInAppWalletHandle &InAppWallet);
};
