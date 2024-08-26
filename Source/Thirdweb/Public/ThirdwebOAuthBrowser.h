// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "ThirdwebWalletHandle.h"
#include "Blueprint/UserWidget.h"

#include "ThirdwebOAuthBrowser.generated.h"

class UWebBrowser;
/**
 * OAuth Browser for Thirdweb
 */
UCLASS()
class THIRDWEB_API UThirdwebOAuthBrowser : public UUserWidget
{
	GENERATED_BODY()

public:

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOAuthSuccess);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOAuthError, FString, Error);


#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override;
#endif

	/** Called when the OAuth flow has completed successfully */
	UPROPERTY(BlueprintAssignable, Category="Thirdweb|OAuth Browser")
	FOnOAuthSuccess OnSuccess;

	/** Called when any error occurs */
	UPROPERTY(BlueprintAssignable, Category="Thirdweb|OAuth Browser")
	FOnOAuthError OnError;

	UFUNCTION(BlueprintCallable, Category="Thirdweb|OAuth Browser")
	void Authenticate(const FWalletHandle& InAppWallet);

protected:
	virtual bool Initialize() override;
private:
	
	FWalletHandle Wallet;

	UPROPERTY(Transient)
	UWebBrowser* Browser;

	UFUNCTION()
	void HandleOnUrlChanged(const FText& InURL);
};
