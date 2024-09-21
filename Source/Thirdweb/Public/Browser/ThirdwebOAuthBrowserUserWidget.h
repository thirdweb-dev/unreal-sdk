// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "ThirdwebWalletHandle.h"

#include "Blueprint/UserWidget.h"

#include "ThirdwebOAuthBrowserUserWidget.generated.h"

UCLASS(DisplayName="OAuth Browser")
class THIRDWEB_API UThirdwebOAuthBrowserUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAuthenticatedDelegate, const FString&, AuthResult);
	UPROPERTY(BlueprintAssignable, Category="Thirdweb|OAuth Browser")
	FOnAuthenticatedDelegate OnAuthenticated;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnErrorDelegate, const FString&, Error);
	UPROPERTY(BlueprintAssignable, Category="Thirdweb|OAuth Browser")
	FOnErrorDelegate OnError;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUrlDelegate, const FString&, URL);
	UPROPERTY(BlueprintAssignable, DisplayName="On URL Changed", Category="Thirdweb|OAuth Browser")
	FUrlDelegate OnUrlChanged;

	UPROPERTY(BlueprintAssignable, Category="Thirdweb|OAuth Browser")
	FUrlDelegate OnPageLoaded;
	
protected:
	/** Automatically collapse the widget when the page is blank */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Browser")
	bool bCollapseWhenBlank = true;

	/** Automatically authenticate on Construct. Only works when created with a Wallet */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Browser")
	bool bAuthenticateOnConstruct = false;
	
	UPROPERTY(BlueprintReadOnly, Transient, Category="Internal", meta=(ExposeOnSpawn=true))
	FWalletHandle Wallet;
	
private:
	UPROPERTY(Transient)
	class UThirdwebOAuthBrowserWidget* Browser = nullptr;

	static const FString BackendUrlPrefix;
	bool bShouldBeVisible = false;
	
public:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void OnWidgetRebuilt() override;
	
#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override;
#endif

protected:
	void SetVisible(const bool bVisible);
	virtual void HandleUrlChanged(const FString& Url);
	virtual void HandlePageLoaded(const FString& Url);
	
public:
	UFUNCTION(BlueprintCallable, Category="Thirdweb|OAuth Browser")
	void Authenticate(const FWalletHandle& InAppWallet);

	/** Returns true if the page content is blank. Normally the case at startup, and mid-oauth flow */
	UFUNCTION(BlueprintPure, Category="Thirdweb|OAuth Browser")
	bool IsBlank() const;
};