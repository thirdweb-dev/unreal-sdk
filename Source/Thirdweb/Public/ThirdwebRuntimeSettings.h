// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "Thirdweb.h"
#include "Engine/DeveloperSettings.h"
#include "Misc/Paths.h"
#include "ThirdwebLog.h"
#include "HAL/FileManager.h"
#include "ThirdwebRuntimeSettings.generated.h"

/**
 * 
 */
UCLASS(Config=Engine, DefaultConfig, meta=(DisplayName="Thirdweb"))
class THIRDWEB_API UThirdwebRuntimeSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category=Config)
	FString ClientID;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category=Config)
	FString BundleID;
	
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category=Config)
	FString SecretKey;

    //~ Optional array of engine signers stored globally for convenience
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category=Config)
	TArray<FString> EngineSigners;
	
	UFUNCTION(BlueprintPure, Category="Thirdweb", DisplayName="Get Thirdweb Runtime Settings")
	static const UThirdwebRuntimeSettings* Get() { return GetDefault<UThirdwebRuntimeSettings>(); }
	
	UFUNCTION(BlueprintPure, Category="Thirdweb")
	static TArray<FString> GetThirdwebGlobalEngineSigners()
	{
		if (const UThirdwebRuntimeSettings* Settings = Get())
		{
			return Settings->EngineSigners;
		}
		return {};
	}

	//~ Gets the first engine signer in the array, if any

	UFUNCTION(BlueprintPure, Category="Thirdweb", meta=(ReturnDisplayName="Signers"))
	static FString GetThirdwebGlobalEngineSigner(bool& bFound)
	{
		bFound = false;
		if (const UThirdwebRuntimeSettings* Settings = Get())
		{
			if (Settings->EngineSigners.Num() > 0)
			{
				bFound = true;
				return Settings->EngineSigners[0];
			}
			
		}
		return TEXT("");
	}

	static FString GetStorageDirectory()
	{
		FString StorageDir = FPaths::Combine(IFileManager::Get().ConvertToAbsolutePathForExternalAppForWrite(*FPaths::ProjectSavedDir()), "thirdweb");
		TW_LOG(Log, TEXT("StorageDir::%s"), *StorageDir)
		return StorageDir;
	}
};
