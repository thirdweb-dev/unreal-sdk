// Copyright (c) 2024 Thirdweb. All Rights Reserved.

// Warning: The information provided in this file is for convenience, and is subject to change at any time.

#pragma once

#include "ThirdwebIPFSUploadResult.generated.h"

USTRUCT(BlueprintType, DisplayName="IPFS Upload Result")
struct THIRDWEB_API FThirdwebIPFSUploadResult
{
	GENERATED_BODY()

	/** IPFS hash of the uploaded content. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Thirdweb|IPFS")
	FString Hash;

	/** Size of the pinned content. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Thirdweb|IPFS")
	FString PinSize;

	/** Timestamp of the upload. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Thirdweb|IPFS")
	FDateTime Timestamp;
	
	/** Timestamp of the upload. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Thirdweb|IPFS")
	FString PreviewUrl;

	static FThirdwebIPFSUploadResult FromJson(const FString& JsonString);
	static FThirdwebIPFSUploadResult FromJson(const TSharedPtr<FJsonObject>& JsonObject);
};