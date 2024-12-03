// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "AsyncTasks/AsyncTaskThirdwebBase.h"
#include "AsyncTaskThirdwebFetchIpfsData.generated.h"

struct FDownloadBytesSuccessDelegate;

UCLASS(Abstract)
class UAsyncTaskThirdwebFetchIpfsBase : public UAsyncTaskThirdwebBase
{
	GENERATED_BODY()

public:
	virtual void Activate() override;

protected:
	UPROPERTY(Transient)
	FString IpfsUri;

	void HandleResponse(const TArray<uint8>& Data);

	virtual void FormatAndBroadcast(const TArray<uint8>& Data)
	{
	}

	virtual void HandleFailed(const FString& Error);
};

/**
 *
 */
UCLASS(Abstract)
class THIRDWEB_API UAsyncTaskThirdwebFetchIpfsRaw : public UAsyncTaskThirdwebFetchIpfsBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, DisplayName="Fetch IPFS Raw", meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject", AutoCreateRefTerm="Args"), Category="Thirdweb|Utils")
	static UAsyncTaskThirdwebFetchIpfsRaw* FetchIpfsRaw(UObject* WorldContextObject, const FString& Uri)
	{
		NEW_TASK
		Task->IpfsUri = Uri;
		RR_TASK
	}

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FFetchIpfsRawDelegate, const TArray<uint8>&, Data, const FString&, Error);

	UPROPERTY(BlueprintAssignable)
	FFetchIpfsRawDelegate Success;

	UPROPERTY(BlueprintAssignable)
	FFetchIpfsRawDelegate Failed;

	virtual void FormatAndBroadcast(const TArray<uint8>& Data) override;
	virtual void HandleFailed(const FString& Error) override;
};

UCLASS(Abstract)
class THIRDWEB_API UAsyncTaskThirdwebFetchIpfsImage : public UAsyncTaskThirdwebFetchIpfsBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, DisplayName="Fetch IPFS Image", meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject", AutoCreateRefTerm="Args"), Category="Thirdweb|Utils")
	static UAsyncTaskThirdwebFetchIpfsImage* FetchIpfsImage(UObject* WorldContextObject, const FString& Uri)
	{
		NEW_TASK
		Task->IpfsUri = Uri;
		RR_TASK
	}

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FFetchIpfsImageDelegate, UTexture2DDynamic*, Data, const FString&, Error);

	UPROPERTY(BlueprintAssignable)
	FFetchIpfsImageDelegate Success;

	UPROPERTY(BlueprintAssignable)
	FFetchIpfsImageDelegate Failed;

	virtual void FormatAndBroadcast(const TArray<uint8>& Data) override;
	virtual void HandleFailed(const FString& Error) override;
};

UCLASS(Abstract)
class THIRDWEB_API UAsyncTaskThirdwebFetchIpfsJson : public UAsyncTaskThirdwebFetchIpfsBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, DisplayName="Fetch IPFS Image", meta=(BlueprintInternalUseOnly="true", WorldContext="WorldContextObject", AutoCreateRefTerm="Args"), Category="Thirdweb|Utils")
	static UAsyncTaskThirdwebFetchIpfsJson* FetchIpfsJson(UObject* WorldContextObject, const FString& Uri)
	{
		NEW_TASK
		Task->IpfsUri = Uri;
		RR_TASK
	}

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FFetchIpfsJsonDelegate, const FString, Data, const FString&, Error);

	UPROPERTY(BlueprintAssignable)
	FFetchIpfsJsonDelegate Success;

	UPROPERTY(BlueprintAssignable)
	FFetchIpfsJsonDelegate Failed;

	virtual void FormatAndBroadcast(const TArray<uint8>& Data) override;
	virtual void HandleFailed(const FString& Error) override;
};
