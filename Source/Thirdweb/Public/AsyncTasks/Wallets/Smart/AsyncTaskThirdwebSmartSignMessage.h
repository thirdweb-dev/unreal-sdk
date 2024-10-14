// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "AsyncTaskThirdwebSmartBase.h"
#include "AsyncTaskThirdwebSmartSignMessage.generated.h"


UCLASS()
class THIRDWEB_API UAsyncTaskThirdwebSmartSignMessage : public UAsyncTaskThirdwebSmartBase
{
	GENERATED_BODY()

public:
	virtual void Activate() override;

	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject", AutoCreateRefTerm="Message"), DisplayName="Sign Message", Category="Thirdweb|Wallets|Smart")
	static UAsyncTaskThirdwebSmartSignMessage* SignMessage(UObject* WorldContextObject, const FSmartWalletHandle& Wallet, const FString& Message);
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSignMessageDelegate, const FString&, SignedMessage, const FString&, Error);
	UPROPERTY(BlueprintAssignable)
	FSignMessageDelegate Success;
	
	UPROPERTY(BlueprintAssignable)
	FSignMessageDelegate Failed;

protected:
	UPROPERTY(Transient)
	FString UnsignedMessage;
	
private:
	UFUNCTION()
	void HandleResponse(const FString& SignedMessage);
	
	UFUNCTION()
	virtual void HandleFailed(const FString& Error);
};
