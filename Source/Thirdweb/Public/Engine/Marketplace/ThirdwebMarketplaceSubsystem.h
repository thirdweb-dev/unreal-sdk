// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "Subsystems/GameInstanceSubsystem.h"
#include "ThirdwebMarketplaceSubsystem.generated.h"

class UThirdwebMarketplace;

UCLASS()
class THIRDWEB_API UThirdwebMarketplaceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	/** Overrides */
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	static void AddMarketplace(const UObject* WorldContextObject, UThirdwebMarketplace* Marketplace);
	static void RemoveMarketplace(const UObject* WorldContextObject, UThirdwebMarketplace* Marketplace);
	static UThirdwebMarketplace* GetMarketplace(const UObject* WorldContextObject, int64 ChainId);
	static TArray<UThirdwebMarketplace*> GetAllMarketplaces(const UObject* WorldContextObject);

	static UThirdwebMarketplaceSubsystem* Get(const UObject* WorldContextObject);
	
private:
	UPROPERTY(Transient)
	TMap<int64, UThirdwebMarketplace*> Marketplaces;
};
