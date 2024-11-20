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

void UThirdwebMarketplaceSubsystem::AddMarketplace(const UObject* WorldContextObject, UThirdwebMarketplace* Marketplace)
{
	if (Marketplace && Marketplace->GetChainId() > 0)
	{
		if (UThirdwebMarketplaceSubsystem* Subsystem = Get(WorldContextObject))
		{
			Subsystem->Marketplaces.Emplace(Marketplace->GetChainId(), Marketplace);
		}
	}
}

void UThirdwebMarketplaceSubsystem::RemoveMarketplace(const UObject* WorldContextObject, UThirdwebMarketplace* Marketplace)
{
	if (Marketplace && Marketplace->GetChainId() > 0)
	{
		if (UThirdwebMarketplaceSubsystem* Subsystem = Get(WorldContextObject))
		{
			if (Subsystem->Marketplaces.Contains(Marketplace->GetChainId()))
			{
				Subsystem->Marketplaces.Remove(Marketplace->GetChainId());
			}
		}
	}
}

UThirdwebMarketplace* UThirdwebMarketplaceSubsystem::GetMarketplace(const UObject* WorldContextObject, int64 ChainId)
{
	if (ChainId > 0)
	{
		if (UThirdwebMarketplaceSubsystem* Subsystem = Get(WorldContextObject))
		{
			if (Subsystem->Marketplaces.Contains(ChainId))
			{
				return *Subsystem->Marketplaces.Find(ChainId);
			}
		}
	}
	return nullptr;
}

TArray<UThirdwebMarketplace*> UThirdwebMarketplaceSubsystem::GetAllMarketplaces(const UObject* WorldContextObject)
{
	if (UThirdwebMarketplaceSubsystem* Subsystem = Get(WorldContextObject))
	{
		TArray<UThirdwebMarketplace*> Result;
		Subsystem->Marketplaces.GenerateValueArray(Result);
		return Result;
	}
	return TArray<UThirdwebMarketplace*>();
}

UThirdwebMarketplaceSubsystem* UThirdwebMarketplaceSubsystem::Get(const UObject* WorldContextObject)
{
	if (UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject))
	{
		return GameInstance->GetSubsystem<UThirdwebMarketplaceSubsystem>();
	}
	return nullptr;
}
