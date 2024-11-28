// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "AsyncTasks/AsyncTaskThirdwebBase.h"
#include "AsyncTaskThirdwebFetchIpfsJson.generated.h"

struct FDownloadBytesSuccessDelegate;
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class THIRDWEB_API UAsyncTaskThirdwebFetchIpfsJson : public UAsyncTaskThirdwebBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, DisplayName="Fetch IPFS JSON", meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject", AutoCreateRefTerm="Args"), Category="Thirdweb|Utils")
	static UAsyncTaskThirdwebFetchIpfsJson* FetchIpfsJson(UObject* WorldContextObject, UPARAM(DisplayName="URI") const FString& Uri);

	virtual void Activate() override;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FFetchIpfsJsonDelegate, const FString&, JSONString, const FString&, Error);
	UPROPERTY(BlueprintAssignable)
	FFetchIpfsJsonDelegate Success;

	UPROPERTY(BlueprintAssignable)
	FFetchIpfsJsonDelegate Failed;

protected:
	UPROPERTY(Transient)
	FString Uri;

private:
	virtual void HandleResponse(const TArray<uint8>& Data);
	void HandleFailed(const FString& Error);
};
