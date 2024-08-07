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
	if (Thirdweb::create_in_app_wallet(Settings->GetClientID(), StringCast<ANSICHAR>(*Email).Get(), Settings->GetSecretKey(), StringCast<ANSICHAR>(*Email).Get(), Settings->GetStorageDirectoryPath(),
	                                   nullptr).AssignResult(Error))
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
	if (Thirdweb::create_in_app_wallet(Settings->GetClientID(), Settings->GetBundleID(), Settings->GetSecretKey(), nullptr, Settings->GetStorageDirectoryPath(),
	                                   StringCast<ANSICHAR>(*ToString(Provider)).Get()).AssignResult(Error))
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
		Settings->ClientID.IsEmpty() ? nullptr : TCHAR_TO_UTF8(*Settings->ClientID),
		Settings->BundleID.IsEmpty() ? nullptr : TCHAR_TO_UTF8(*Settings->BundleID),
		Settings->SecretKey.IsEmpty() ? nullptr : TCHAR_TO_UTF8(*Settings->SecretKey),
		PersonalWallet.ID,
		TCHAR_TO_UTF8(*FString::Printf(TEXT("%lld"), ChainID)),
		bGasless,
		Factory.IsEmpty() ? nullptr : TCHAR_TO_UTF8(*Factory),
		AccountOverride.IsEmpty() ? nullptr : TCHAR_TO_UTF8(*AccountOverride)
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
