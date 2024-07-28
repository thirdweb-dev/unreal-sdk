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


EFunctionResult UThirdwebSubsystem::BP_CreateSmartWallet(const FWalletHandle& LocalWallet, FWalletHandle& SmartWallet, FString& Error, const int64 ChainID, const bool bGasless, const FString& Factory,
                                                         const FString& AccountOverride)
{
	return CreateSmartWallet(LocalWallet, ChainID, bGasless, Factory, AccountOverride, SmartWallet, Error) ? EFunctionResult::Success : EFunctionResult::Failed;
}

bool UThirdwebSubsystem::CreateSmartWallet(const FWalletHandle& LocalWallet, const int64 ChainID, bool bGasless, const FString& Factory, const FString& AccountOverride, FWalletHandle& SmartWallet,
                                           FString& Error)
{
	if (Thirdweb::create_smart_wallet(Settings->GetClientID(), Settings->GetBundleID(), Settings->GetSecretKey(), LocalWallet.ID, StringCast<ANSICHAR>(*FString::Printf(TEXT("%lld"), ChainID)).Get(),
	                                  bGasless, StringCast<ANSICHAR>(*Factory).Get(), StringCast<ANSICHAR>(*AccountOverride).Get()).AssignResult(Error))
	{
		SmartWallet = FWalletHandle(FWalletHandle::Smart, Error);
		Error.Empty();
		return true;
	}
	return false;
}

EFunctionResult UThirdwebSubsystem::BP_GetSmartWalletAdmins(const FWalletHandle& Wallet, FString& Output, FString& Error)
{
	return GetSmartWalletAdmins(Wallet, Output, Error) ? EFunctionResult::Success : EFunctionResult::Failed;
}

bool UThirdwebSubsystem::GetSmartWalletAdmins(const FWalletHandle& Wallet, FString& Output, FString& Error)
{
	if (Wallet.Type == FWalletHandle::Smart)
	{
		if (Thirdweb::smart_wallet_get_all_admins(Wallet.ID).AssignResult(Error))
		{
			Output = Error;
			Error.Empty();
			return true;
		}
	}
	Error = TEXT("Not a networked wallet");
	return false;
}

EFunctionResult UThirdwebSubsystem::BP_GetSmartWalletActiveSigners(const FWalletHandle& Wallet, FString& Output, FString& Error)
{
	return GetSmartWalletActiveSigners(Wallet, Output, Error) ? EFunctionResult::Success : EFunctionResult::Failed;
}

bool UThirdwebSubsystem::GetSmartWalletActiveSigners(const FWalletHandle& Wallet, FString& Output, FString& Error)
{
	if (Wallet.Type == FWalletHandle::Smart)
	{
		if (Thirdweb::smart_wallet_get_all_active_signers(Wallet.ID).AssignResult(Error))
		{
			Output = Error;
			Error.Empty();
			return true;
		}
	}
	Error = TEXT("Not a networked wallet");
	return false;
}

EFunctionResult UThirdwebSubsystem::BP_CreateSmartWalletSessionKey(const FWalletHandle& Wallet, const FString& SignerAddress, const bool IsAdmin, const TArray<FString>& ApprovedTargets,
                                                                   const FString& NativeTokenLimitPerTransactionInWei, const FDateTime& PermissionStart, const FDateTime& PermissionEnd,
                                                                   const FDateTime& RequestValidityStart, const FDateTime& RequestValidityEnd,
                                                                   FString& Key, FString& Error)
{
	return CreateSmartWalletSessionKey(Wallet, SignerAddress, IsAdmin, ApprovedTargets, NativeTokenLimitPerTransactionInWei, PermissionStart, PermissionEnd, RequestValidityStart, RequestValidityEnd,
	                                   Key, Error)
		       ? EFunctionResult::Success
		       : EFunctionResult::Failed;
}

bool UThirdwebSubsystem::CreateSmartWalletSessionKey(const FWalletHandle& Wallet, const FString& SignerAddress, const bool bIsAdmin, const TArray<FString>& ApprovedTargets,
                                                     const FString& NativeTokenLimitPerTransactionInWei, const FDateTime& PermissionStart, const FDateTime& PermissionEnd,
                                                     const FDateTime& RequestValidityStart, const FDateTime& RequestValidityEnd, FString& Key, FString& Error)
{
	if (Wallet.Type != FWalletHandle::Smart)
	{
		Error = TEXT("Not a networked wallet");
		return false;
	}

	TArray<const char*> ApprovedTargetsCArray;
	for (const FString& Target : ApprovedTargets)
	{
		ApprovedTargetsCArray.Add(StringCast<ANSICHAR>(*Target).Get());
	}
	if (Thirdweb::smart_wallet_create_session_key(
		Wallet.ID,
		StringCast<ANSICHAR>(*SignerAddress).Get(),
		StringCast<ANSICHAR>(bIsAdmin ? TEXT("1") : TEXT("0")).Get(),
		ApprovedTargetsCArray.GetData(),
		ApprovedTargetsCArray.Num(),
		StringCast<ANSICHAR>(*NativeTokenLimitPerTransactionInWei).Get(),
		StringCast<ANSICHAR>(*FString::FromInt(PermissionStart.ToUnixTimestampDecimal())).Get(),
		StringCast<ANSICHAR>(*FString::FromInt(PermissionEnd.ToUnixTimestampDecimal())).Get(),
		StringCast<ANSICHAR>(*FString::FromInt(RequestValidityStart.ToUnixTimestampDecimal())).Get(),
		StringCast<ANSICHAR>(*FString::FromInt(RequestValidityEnd.ToUnixTimestampDecimal())).Get()
	).AssignResult(Error))
	{
		Key = Error;
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
