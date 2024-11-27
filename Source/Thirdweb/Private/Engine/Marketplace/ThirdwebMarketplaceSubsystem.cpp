// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "Engine/Marketplace/ThirdwebMarketplaceSubsystem.h"

#include "Engine/GameInstance.h"
#include "Engine/Marketplace/ThirdwebMarketplace.h"
#include "Kismet/GameplayStatics.h"

void UThirdwebMarketplaceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UThirdwebMarketplaceSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

UThirdwebMarketplace* UThirdwebMarketplaceSubsystem::AddMarketplace(const UObject* WorldContextObject, const int64 ChainId, const FString& Address)
{
	if (ChainId > 0 && !Address.IsEmpty())
	{
		if (UThirdwebMarketplaceSubsystem* Subsystem = Get(WorldContextObject))
		{
			return Subsystem->RegisterMarketplace(WorldContextObject, NewObject<UThirdwebMarketplace>(Subsystem));
		}
	}
	return nullptr;
}

UThirdwebMarketplace* UThirdwebMarketplaceSubsystem::RegisterMarketplace(const UObject* WorldContextObject, UThirdwebMarketplace* Marketplace)
{
	if (Marketplace && Marketplace->IsValid())
	{
		if (UThirdwebMarketplaceSubsystem* Subsystem = Get(WorldContextObject))
		{
			int64 ChainId = Marketplace->GetChainId();
			if (!Subsystem->Marketplaces.Contains(ChainId))
			{
				Subsystem->Marketplaces.Add(ChainId);
			}
			return Subsystem->Marketplaces[ChainId].FindOrAdd(Marketplace->GetContractAddress(), Marketplace);
		}
	}
	return nullptr;
}

void UThirdwebMarketplaceSubsystem::DeregisterMarketplace(const UObject* WorldContextObject, UThirdwebMarketplace* Marketplace)
{
	if (Marketplace && Marketplace->IsValid())
	{
		if (UThirdwebMarketplaceSubsystem* Subsystem = Get(WorldContextObject))
		{
			if (int64 ChainId = Marketplace->GetChainId(); Subsystem->Marketplaces.Contains(ChainId))
			{
				Subsystem->Marketplaces[ChainId].Remove(Marketplace->GetContractAddress());
			}
		}
	}
}

UThirdwebMarketplace* UThirdwebMarketplaceSubsystem::FindMarketplace(const UObject* WorldContextObject, const int64 ChainId, const FString& Address)
{
	if (ChainId > 0)
	{
		if (UThirdwebMarketplaceSubsystem* Subsystem = Get(WorldContextObject))
		{
			if (Subsystem->Marketplaces.Contains(ChainId))
			{
				return *Subsystem->Marketplaces[ChainId].Find(Address);
			}
		}
	}
	return nullptr;
}

TArray<UThirdwebMarketplace*> UThirdwebMarketplaceSubsystem::GetAllMarketplaces(const UObject* WorldContextObject)
{
	if (UThirdwebMarketplaceSubsystem* Subsystem = Get(WorldContextObject))
	{
		TArray<TMap<FString, UThirdwebMarketplace*>> ChainMarketplaceMaps;
		Subsystem->Marketplaces.GenerateValueArray(ChainMarketplaceMaps);
		
		TArray<UThirdwebMarketplace*> Result;
		for (TMap ChainMarketplaceMap : ChainMarketplaceMaps)
		{
			
			TArray<UThirdwebMarketplace*> ChainResult;
			ChainMarketplaceMap.GenerateValueArray(ChainResult);
			Result.Append(ChainResult);
		}
		return Result;
	}
	return {};
}

UThirdwebMarketplaceSubsystem* UThirdwebMarketplaceSubsystem::Get(const UObject* WorldContextObject)
{
	if (UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject))
	{
		return GameInstance->GetSubsystem<UThirdwebMarketplaceSubsystem>();
	}
	return nullptr;
}
