// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "AsyncTasks/AsyncTaskThirdwebBase.h"
#include "AsyncTaskThirdwebFetchIpfsData.generated.h"

struct FDownloadBytesSuccessDelegate;
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class THIRDWEB_API UAsyncTaskThirdwebFetchIpfsData : public UAsyncTaskThirdwebBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, DisplayName="Fetch IPFS Data", meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject", AutoCreateRefTerm="Args"), Category="Thirdweb|Utils")
	static UAsyncTaskThirdwebFetchIpfsData* FetchIpfsData(UObject* WorldContextObject, UPARAM(DisplayName="URI") const FString& Uri);

	virtual void Activate() override;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FFetchIpfsDataDelegate, const TArray<uint8>&, Data, const FString&, Error);
	UPROPERTY(BlueprintAssignable)
	FFetchIpfsDataDelegate Success;

	UPROPERTY(BlueprintAssignable)
	FFetchIpfsDataDelegate Failed;

protected:
	UPROPERTY(Transient)
	FString Uri;

private:
	virtual void HandleResponse(const TArray<uint8>& Data);
	void HandleFailed(const FString& Error);
};
