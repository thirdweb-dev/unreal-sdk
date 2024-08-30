// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "ThirdwebWalletHandle.h"
#include "Blueprint/UserWidget.h"
#include "ThirdwebOAuthBrowserUserWidget.generated.h"

UCLASS()
class THIRDWEB_API UThirdwebOAuthBrowserUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void OnWidgetRebuilt() override;
#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override;
#endif

	UFUNCTION(BlueprintCallable, Category="Thirdweb|OAuth Browser")
	void Authenticate(const FWalletHandle& InAppWallet);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSuccess);
	UPROPERTY(BlueprintAssignable, Category="Thirdweb|OAuth Browser")
	FOnSuccess OnSuccess;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnError, const FString&, Error);
	UPROPERTY(BlueprintAssignable, Category="Thirdweb|OAuth Browser")
	FOnError OnError;

private:
	UPROPERTY(Transient)
	class UThirdwebOAuthBrowserWidget* Browser = nullptr;
	
	UPROPERTY(Transient)
	FWalletHandle Wallet;

	UFUNCTION()
	void HandleUrlChanged(const FString& Url);
	
	static const FString BackendUrlPrefix;

	void SetVisible(const bool bVisible);
};