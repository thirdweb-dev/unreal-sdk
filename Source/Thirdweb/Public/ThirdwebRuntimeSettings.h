// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "Thirdweb.h"
#include "ThirdwebLog.h"

#include "Engine/DeveloperSettings.h"

#include "HAL/FileManager.h"

#include "Misc/Paths.h"

#include "ThirdwebRuntimeSettings.generated.h"

enum class EThirdwebAuthenticationMethod : uint8;

/**
 * This class acts as a configuration holder to manage various settings required for the runtime environment in the Thirdweb system.
 */
UCLASS(Config=Engine, DefaultConfig, meta=(DisplayName="Thirdweb"))
class THIRDWEB_API UThirdwebRuntimeSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UThirdwebRuntimeSettings();
	
	/** Toggles configuration between ClientID + BundleID and SecretKey. Thirdweb recommends using ClientID over SecretKey for security. */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category=Config)
	EThirdwebAuthenticationMethod AuthenticationMethod;
	
	/** Stores the client identifier. */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category=Config, meta=(EditCondition="AuthenticationMethod==EThirdwebAuthenticationMethod::ClientID", EditConditionHides))
	FString ClientID;
	
	/** Stores the bundle identifier. */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category=Config, meta=(EditCondition="AuthenticationMethod==EThirdwebAuthenticationMethod::ClientID", EditConditionHides))
	FString BundleID;

	/** Stores the secret key. */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category=Config, meta=(EditCondition="AuthenticationMethod==EThirdwebAuthenticationMethod::SecretKey", EditConditionHides))
	FString SecretKey;

    /** Optional array of engine signers stored globally for convenience */
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

	/** Gets the first global engine signer in the array, if any */
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
		FString StorageDir = FPaths::Combine(IFileManager::Get().ConvertToAbsolutePathForExternalAppForWrite(*FPaths::ProjectSavedDir()), "Thirdweb", "InAppWallet");
		TW_LOG(Verbose, TEXT("StorageDir::%s"), *StorageDir)
		return StorageDir;
	}
};

