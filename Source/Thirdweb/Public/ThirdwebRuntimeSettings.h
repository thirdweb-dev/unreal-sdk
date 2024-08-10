// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "Thirdweb.h"
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
	const char* GetClientID() const { return Thirdweb::GetOrNull(ClientID); }
	const char* GetBundleID() const { return Thirdweb::GetOrNull(BundleID); }
	const char* GetSecretKey() const { return Thirdweb::GetOrNull(SecretKey); }
	const char* GetStorageDirectoryPath() const { return Thirdweb::GetOrNull(StorageDirectoryPath); }
};
