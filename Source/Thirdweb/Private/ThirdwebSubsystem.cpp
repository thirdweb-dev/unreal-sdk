// Copyright (c) 2024 Thirdweb. All Rights Reserved.

// ReSharper disable CppUE4BlueprintCallableFunctionMayBeStatic

#include "ThirdwebSubsystem.h"
#include "HttpServerModule.h"
#include "IHttpRouter.h"
#include "Thirdweb.h"
#include "ThirdwebCommon.h"
#include "ThirdwebLog.h"
#include "ThirdwebRuntimeSettings.h"
#include "ThirdwebWalletHandle.h"
#include "Engine/StreamableManager.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/DefaultValueHelper.h"
#include "Engine/GameInstance.h"

void UThirdwebSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	Settings = GetDefault<UThirdwebRuntimeSettings>();
	TW_LOG(Log, TEXT("ThirdwebSubsystem::Intitialize::Subsystem initialization complete"))
}

void UThirdwebSubsystem::Deinitialize()
{
	Super::Deinitialize();
	TW_LOG(Log, TEXT("ThirdwebSubsystem::Deinitialize::Subsystem deinitialization complete"))
}

EFunctionResult UThirdwebSubsystem::BP_CreateInAppEmailWallet(const FString& Email, FWalletHandle& Wallet, FString& Error)
{
	return CreateInAppEmailWallet(Email, Wallet, Error) ? EFunctionResult::Success : EFunctionResult::Failed;
}

bool UThirdwebSubsystem::CreateInAppEmailWallet(const FString& Email, FWalletHandle& Wallet, FString& Error)
{
	if (Thirdweb::create_in_app_wallet(
		TO_RUST_STRING(Settings->ClientID),
		TO_RUST_STRING(Settings->BundleID),
		TO_RUST_STRING(Settings->SecretKey),
		TO_RUST_STRING(Email),
		TO_RUST_STRING(Settings->GetStorageDirectory()),
		nullptr
	).AssignResult(Error))
	{
		Wallet = FWalletHandle(FWalletHandle::InApp, Error);
		Error.Empty();
		return true;
	}
	return false;
}

EFunctionResult UThirdwebSubsystem::BP_CreateInAppOAuthWallet(const EThirdwebOAuthProvider Provider, FWalletHandle& Wallet, FString& Error)
{
	return CreateInAppOAuthWallet(Provider, Wallet, Error) ? EFunctionResult::Success : EFunctionResult::Failed;
}

bool UThirdwebSubsystem::CreateInAppOAuthWallet(const EThirdwebOAuthProvider Provider, FWalletHandle& Wallet, FString& Error)
{
	if (Thirdweb::create_in_app_wallet(
		TO_RUST_STRING(Settings->ClientID),
		TO_RUST_STRING(Settings->BundleID),
		TO_RUST_STRING(Settings->SecretKey),
		nullptr,
		TO_RUST_STRING(Settings->GetStorageDirectory()),
		TO_RUST_STRING(ToString(Provider))
	).AssignResult(Error))
	{
		Wallet = FWalletHandle(FWalletHandle::InApp, Error);
		Error.Empty();
		return true;
	}
	return false;
}


EFunctionResult UThirdwebSubsystem::BP_CreateSmartWallet(const FWalletHandle& PersonalWallet, FWalletHandle& SmartWallet, FString& Error, const int64 ChainID, const bool bGasless,
                                                         const FString& Factory,
                                                         const FString& AccountOverride)
{
	return CreateSmartWallet(PersonalWallet, ChainID, bGasless, Factory, AccountOverride, SmartWallet, Error) ? EFunctionResult::Success : EFunctionResult::Failed;
}

bool UThirdwebSubsystem::CreateSmartWallet(const FWalletHandle& PersonalWallet, const int64 ChainID, const bool bGasless, const FString& Factory, const FString& AccountOverride,
                                           FWalletHandle& SmartWallet,
                                           FString& Error)
{
	if (Thirdweb::create_smart_wallet(
		TO_RUST_STRING(Settings->ClientID),
		TO_RUST_STRING(Settings->BundleID),
		TO_RUST_STRING(Settings->SecretKey),
		PersonalWallet.ID,
		TO_RUST_STRING(FString::Printf(TEXT("%lld"), ChainID)),
		bGasless,
		TO_RUST_STRING(Factory),
		TO_RUST_STRING(AccountOverride)
	).AssignResult(Error))
	{
		SmartWallet = FWalletHandle(FWalletHandle::Smart, Error);
		Error.Empty();
		return true;
	}
	return false;
}

UThirdwebSubsystem* UThirdwebSubsystem::Get(const UObject* WorldContextObject)
{
	if (UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject))
	{
		return GameInstance->GetSubsystem<UThirdwebSubsystem>();
	}
	return nullptr;
}
