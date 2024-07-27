// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "Engine/DeveloperSettings.h"
#include "ThirdwebRuntimeSettings.generated.h"

class UThirdwebSubsystem;
/**
 * 
 */
UCLASS(Config=Engine, DefaultConfig, meta=(DisplayName="Thirdweb"))
class THIRDWEB_API UThirdwebRuntimeSettings : public UDeveloperSettings
{
	GENERATED_BODY()

	friend UThirdwebSubsystem;
	
public:
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category=Config)
	FString ClientID;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category=Config)
	FString BundleID;
	
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category=Config)
	FString SecretKey;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category=Config)
	FString StorageDirectoryPath;

	UFUNCTION(BlueprintPure, Category="Thirdweb", DisplayName="Get Thirdweb Runtime Settings")
	static const UThirdwebRuntimeSettings* Get() { return GetDefault<UThirdwebRuntimeSettings>(); }

private:
	const char* GetClientID() const { return StringCast<ANSICHAR>(*ClientID).Get(); }
	const char* GetBundleID() const { return StringCast<ANSICHAR>(*BundleID).Get(); }
	const char* GetSecretKey() const { return StringCast<ANSICHAR>(*SecretKey).Get(); }
	const char* GetStorageDirectoryPath() const { return StringCast<ANSICHAR>(*StorageDirectoryPath).Get(); }
};
