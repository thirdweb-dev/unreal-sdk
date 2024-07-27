// Copyright (c) 2024 Thirdweb. All Rights Reserved.

// ReSharper disable CppUE4BlueprintCallableFunctionMayBeStatic

#include "ThirdwebSubsystem.h"
#include "HttpServerModule.h"
#include "IHttpRouter.h"
#include "Thirdweb.h"
#include "ThirdwebLog.h"
#include "ThirdwebRuntimeSettings.h"
#include "ThirdwebWalletHandle.h"
#include "Engine/StreamableManager.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/DefaultValueHelper.h"

void UThirdwebSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	TW_LOG(Log, TEXT("ThirdwebSubsystem::Intitialize::Subsystem initialization complete"))
}

void UThirdwebSubsystem::Deinitialize()
{
	Super::Deinitialize();
	TW_LOG(Log, TEXT("ThirdwebSubsystem::Deinitialize::Subsystem deinitialization complete"))
}

EFunctionResult UThirdwebSubsystem::BP_CreatePrivateKeyWallet(const FString& PrivateKey, FWalletHandle& Wallet, FString& Error)
{
	bool bCanRetry;
	if (CreatePrivateKeyWallet(PrivateKey, bCanRetry, Wallet, Error))
	{
		return EFunctionResult::Success;
	}
	return bCanRetry ? EFunctionResult::FailedCanRetry : EFunctionResult::Failed;
}

bool UThirdwebSubsystem::CreatePrivateKeyWallet(const FString& PrivateKey, bool& bCanRetry, FWalletHandle& Wallet, FString& Error)
{
	FString Output;
	if (Thirdweb::create_private_key_wallet(StringCast<ANSICHAR>(*PrivateKey).Get()).AssignResult(bCanRetry, Output))
	{
		Wallet = FWalletHandle(FWalletHandle::PrivateKey, Output);
		return true;
	}
	Wallet = FWalletHandle::Invalid;
	Error = Output;
	return false;
}

EFunctionResult UThirdwebSubsystem::BP_GeneratePrivateKeyWallet(FWalletHandle& Wallet, FString& Error)
{
	bool bCanRetry;
	if (GeneratePrivateKeyWallet(Wallet, bCanRetry, Error))
	{
		return EFunctionResult::Success;
	}
	return bCanRetry ? EFunctionResult::FailedCanRetry : EFunctionResult::Failed;
}

bool UThirdwebSubsystem::GeneratePrivateKeyWallet(FWalletHandle& Wallet, bool& CanRetry, FString& Error)
{
	FString Output;
	if (Thirdweb::generate_private_key_wallet().AssignResult(CanRetry, Output))
	{
		Wallet = FWalletHandle(FWalletHandle::PrivateKey, Output);
		return true;
	}
	Wallet = FWalletHandle::Invalid;
	Error = Output;
	return false;
}

EFunctionResult UThirdwebSubsystem::BP_CreateInAppEmailWallet(const FString& Email, FWalletHandle& Wallet, FString& Error)
{
	bool bCanRetry;
	if (CreateInAppWallet(Email, TEXT(""), bCanRetry, Wallet, Error))
	{
		return EFunctionResult::Success;
	}
	return bCanRetry ? EFunctionResult::FailedCanRetry : EFunctionResult::Failed;
}

EFunctionResult UThirdwebSubsystem::BP_CreateInAppOAuthWallet(const FString& OAuthMethod, FWalletHandle& Wallet, FString& Error)
{
	bool bCanRetry;
	if (CreateInAppWallet(TEXT(""), OAuthMethod, bCanRetry, Wallet, Error))
	{
		return EFunctionResult::Success;
	}
	return bCanRetry ? EFunctionResult::FailedCanRetry : EFunctionResult::Failed;
}

bool UThirdwebSubsystem::CreateInAppWallet(const FString& Email, const FString& OAuthMethod, bool& bCanRetry, FWalletHandle& Wallet, FString& Error)
{
	const UThirdwebRuntimeSettings* Settings = GetDefault<UThirdwebRuntimeSettings>();
	FString Output;
	if (Thirdweb::create_in_app_wallet(
		Settings->GetClientID(),
		StringCast<ANSICHAR>(*Email).Get(),
		Settings->GetSecretKey(),
		StringCast<ANSICHAR>(*Email).Get(),
		Settings->GetStorageDirectoryPath(),
		StringCast<ANSICHAR>(*OAuthMethod).Get()
	).AssignResult(bCanRetry, Output))
	{
		Wallet = FWalletHandle(FWalletHandle::InApp, Output);
		return true;
	}
	Wallet = FWalletHandle::Invalid;
	Error = Output;
	return false;
}

EFunctionResult UThirdwebSubsystem::BP_SendOTP(const FWalletHandle& Wallet, FString& Error)
{
	bool bCanRetry;
	if (SendOTP(Wallet, bCanRetry, Error))
	{
		return EFunctionResult::Success;
	}
	return bCanRetry ? EFunctionResult::FailedCanRetry : EFunctionResult::Failed;
}

bool UThirdwebSubsystem::SendOTP(const FWalletHandle& Wallet, bool& CanRetry, FString& Error)
{
	return Thirdweb::in_app_wallet_send_otp(Wallet.ID).AssignResult(CanRetry, Error, true);
}

EFunctionResult UThirdwebSubsystem::BP_VerifyOTP(const FWalletHandle& Wallet, const FString& OTP, FString& Error)
{
	bool bCanRetry;
	if (VerifyOTP(Wallet, OTP, bCanRetry, Error))
	{
		return EFunctionResult::Success;
	}
	return bCanRetry ? EFunctionResult::FailedCanRetry : EFunctionResult::Failed;
}

bool UThirdwebSubsystem::VerifyOTP(const FWalletHandle& Wallet, const FString& OTP, bool& CanRetry, FString& Error)
{
	return Thirdweb::in_app_wallet_verify_otp(Wallet.ID, StringCast<ANSICHAR>(*OTP).Get()).AssignResult(CanRetry, Error, true);
}

EFunctionResult UThirdwebSubsystem::BP_FetchOAuthLoginLink(const FWalletHandle& Wallet, const FString& RedirectUrl, FString& LoginLink, FString& Error)
{
	bool bCanRetry;
	if (FetchOAuthLoginLink(Wallet, RedirectUrl, bCanRetry, LoginLink, Error))
	{
		return EFunctionResult::Success;
	}
	return bCanRetry ? EFunctionResult::FailedCanRetry : EFunctionResult::Failed;
}

bool UThirdwebSubsystem::FetchOAuthLoginLink(const FWalletHandle& Wallet, const FString& RedirectUrl, bool& CanRetry, FString& LoginLink, FString& Error)
{
	FString Output;
	if (Thirdweb::in_app_wallet_fetch_oauth_login_link(Wallet.ID, StringCast<ANSICHAR>(*RedirectUrl).Get()).AssignResult(CanRetry, Output))
	{
		LoginLink = Output;
		return true;
	}
	Error = Output;
	return false;
}

EFunctionResult UThirdwebSubsystem::BP_SignInWithOAuth(const FWalletHandle& Wallet, const FString& AuthResult, FString& Output)
{
	bool bCanRetry;
	if (SignInWithOAuth(Wallet, AuthResult, bCanRetry, Output))
	{
		return EFunctionResult::Success;
	}
	return bCanRetry ? EFunctionResult::FailedCanRetry : EFunctionResult::Failed;
}

bool UThirdwebSubsystem::SignInWithOAuth(const FWalletHandle& Wallet, const FString& AuthResult, bool& CanRetry, FString& Output)
{
	return Thirdweb::in_app_wallet_sign_in_with_oauth(Wallet.ID, StringCast<ANSICHAR>(*AuthResult).Get()).AssignResult(CanRetry, Output);
}

void UThirdwebSubsystem::LoginWithOAuthDefault(const FWalletHandle& Wallet, const FOauthResponseDelegate& SuccessDelegate, const FOauthResponseDelegate& ErrorDelegate)
{
	FString RedirectUrl = TEXT("http://localhost:8789/callback");
	bool OAuthCanRetry;
	FString OAuthOutput;
	bool OAuthSuccess = Thirdweb::in_app_wallet_fetch_oauth_login_link(Wallet.ID, StringCast<ANSICHAR>(*RedirectUrl).Get())
		.AssignResult(OAuthCanRetry, OAuthOutput);

	if (!OAuthSuccess)
	{
		TW_LOG(Error, TEXT("Failed to fetch OAuth login link."));
		if (ErrorDelegate.IsBound())
		{
			ErrorDelegate.Execute(OAuthOutput);
		}
	}
	if (SuccessDelegate.IsBound())
	{
		SuccessDelegate.Execute(OAuthOutput);
	}
}

EFunctionResult UThirdwebSubsystem::BP_CreateSmartWallet(const FWalletHandle& PersonalWallet, const FString& ChainID, const bool bGasless, const FString& Factory, const FString& AccountOverride,
                                                         FWalletHandle& Wallet, FString& Error)
{
	bool bCanRetry;
	if (CreateSmartWallet(PersonalWallet, ChainID, bGasless, Factory, AccountOverride, Wallet, bCanRetry, Error))
	{
		return EFunctionResult::Success;
	}
	return bCanRetry ? EFunctionResult::FailedCanRetry : EFunctionResult::Failed;
}

bool UThirdwebSubsystem::CreateSmartWallet(const FWalletHandle& PersonalWallet, const FString& ChainID, const bool bGasless, const FString& Factory, const FString& AccountOverride,
                                           FWalletHandle& Wallet, bool& CanRetry, FString& Error)
{
	const UThirdwebRuntimeSettings* Settings = GetDefault<UThirdwebRuntimeSettings>();
	FString Output;
	bool bSuccess = Thirdweb::create_smart_wallet(
		Settings->GetClientID(),
		Settings->GetBundleID(),
		Settings->GetSecretKey(),
		PersonalWallet.ID,
		StringCast<ANSICHAR>(*ChainID).Get(),
		bGasless,
		StringCast<ANSICHAR>(*Factory).Get(),
		StringCast<ANSICHAR>(*AccountOverride).Get()
	).AssignResult(CanRetry, Output);
	if (bSuccess)
	{
		Wallet = FWalletHandle(FWalletHandle::Smart, Output);
		return true;
	}
	Wallet = FWalletHandle::Invalid;
	Error = Output;
	return false;
}

ESmartWalletDeployedFunctionResult UThirdwebSubsystem::BP_IsSmartWalletDeployed(const FWalletHandle& Wallet, FString& Error)
{
	bool bCanRetry;
	if (bool bDeployed; IsSmartWalletDeployed(Wallet, bDeployed, bCanRetry, Error))
	{
		return bDeployed ? ESmartWalletDeployedFunctionResult::Deployed : ESmartWalletDeployedFunctionResult::NotDeployed;
	}
	return bCanRetry ? ESmartWalletDeployedFunctionResult::FailedCanRetry : ESmartWalletDeployedFunctionResult::Failed;
}

bool UThirdwebSubsystem::IsSmartWalletDeployed(const FWalletHandle& Wallet, bool& bDeployed, bool& CanRetry, FString& Error)
{
	FString Output;
	if (Thirdweb::smart_wallet_is_deployed(Wallet.ID).AssignResult(CanRetry, Output))
	{
		bDeployed = Output.ToBool();
		return true;
	}
	Error = Output;
	return false;
}

EFunctionResult UThirdwebSubsystem::BP_GetSmartWalletAdmins(const FWalletHandle& Wallet, FString& Output)
{
	bool bCanRetry;
	if (GetSmartWalletAdmins(Wallet, bCanRetry, Output))
	{
		return EFunctionResult::Success;
	}
	return bCanRetry ? EFunctionResult::FailedCanRetry : EFunctionResult::Failed;
}

bool UThirdwebSubsystem::GetSmartWalletAdmins(const FWalletHandle& Wallet, bool& CanRetry, FString& Output)
{
	return Thirdweb::smart_wallet_get_all_admins(Wallet.ID).AssignResult(CanRetry, Output);
}

EFunctionResult UThirdwebSubsystem::BP_GetSmartWalletActiveSigners(const FWalletHandle& Wallet, FString& Output)
{
	bool bCanRetry;
	if (GetSmartWalletActiveSigners(Wallet, bCanRetry, Output))
	{
		return EFunctionResult::Success;
	}
	return bCanRetry ? EFunctionResult::FailedCanRetry : EFunctionResult::Failed;
}

bool UThirdwebSubsystem::GetSmartWalletActiveSigners(const FWalletHandle& Wallet, bool& CanRetry, FString& Output)
{
	return Thirdweb::smart_wallet_get_all_active_signers(Wallet.ID).AssignResult(CanRetry, Output);
}

EFunctionResult UThirdwebSubsystem::BP_CreateSmartWalletSessionKey(const FWalletHandle& Wallet, const FString& SignerAddress, const bool IsAdmin, const TArray<FString>& ApprovedTargets,
                                                                   const FString& NativeTokenLimitPerTransactionInWei, const FDateTime& PermissionStart, const FDateTime& PermissionEnd,
                                                                   const FDateTime& RequestValidityStart, const FDateTime& RequestValidityEnd,
                                                                   FString& Key, FString& Error)
{
	bool bCanRetry;
	if (CreateSmartWalletSessionKey(Wallet, SignerAddress, IsAdmin, ApprovedTargets, NativeTokenLimitPerTransactionInWei, PermissionStart, PermissionEnd, RequestValidityStart, RequestValidityEnd,
	                                bCanRetry, Key, Error))
	{
		return EFunctionResult::Success;
	}
	return bCanRetry ? EFunctionResult::FailedCanRetry : EFunctionResult::Failed;
}

bool UThirdwebSubsystem::CreateSmartWalletSessionKey(const FWalletHandle& Wallet, const FString& SignerAddress, const bool bIsAdmin, const TArray<FString>& ApprovedTargets,
                                                     const FString& NativeTokenLimitPerTransactionInWei, const FDateTime& PermissionStart, const FDateTime& PermissionEnd,
                                                     const FDateTime& RequestValidityStart, const FDateTime& RequestValidityEnd, bool& CanRetry, FString& Key, FString& Error)
{
	TArray<const char*> ApprovedTargetsCArray;
	for (const FString& Target : ApprovedTargets)
	{
		ApprovedTargetsCArray.Add(StringCast<ANSICHAR>(*Target).Get());
	}
	FString Output;
	bool bSuccess = Thirdweb::smart_wallet_create_session_key(
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
	).AssignResult(CanRetry, Output);
	if (bSuccess)
	{
		Key = Output;
		return true;
	}
	Error = Output;
	return false;
}

EFunctionResult UThirdwebSubsystem::BP_GetWalletAddress(const FWalletHandle& Wallet, FString& Address, FString& Error)
{
	bool bCanRetry;
	if (GetWalletAddress(Wallet, bCanRetry, Address, Error))
	{
		return EFunctionResult::Success;
	}
	return bCanRetry ? EFunctionResult::FailedCanRetry : EFunctionResult::Failed;
}

bool UThirdwebSubsystem::GetWalletAddress(const FWalletHandle& Wallet, bool& CanRetry, FString& Address, FString& Error)
{
	FString Output;
	if (Thirdweb::get_wallet_address(Wallet.ID).AssignResult(CanRetry, Output))
	{
		Address = Output;
		return true;
	}
	Error = Output;
	return false;
}

EFunctionResult UThirdwebSubsystem::BP_SignMessage(const FWalletHandle& Wallet, const FString& Message, FString& Result, FString& Error)
{
	bool bCanRetry;
	if (SignMessage(Wallet, Message, bCanRetry, Result, Error))
	{
		return EFunctionResult::Success;
	}
	return bCanRetry ? EFunctionResult::FailedCanRetry : EFunctionResult::Failed;
}

bool UThirdwebSubsystem::SignMessage(const FWalletHandle& Wallet, const FString& Message, bool& CanRetry, FString& Result, FString& Error)
{
	FString Output;
	if (Thirdweb::sign_message(Wallet.ID, StringCast<ANSICHAR>(*Message).Get()).AssignResult(CanRetry, Output))
	{
		Result = Output;
		return true;
	}
	Error = Output;
	return false;
}

EWalletConnectedFunctionResult UThirdwebSubsystem::BP_IsConnected(const FWalletHandle& Wallet, FString& Error)
{
	bool bCanRetry;
	if (bool bIsConnected; IsConnected(Wallet, bIsConnected, bCanRetry, Error))
	{
		return bIsConnected ? EWalletConnectedFunctionResult::Connected : EWalletConnectedFunctionResult::Disconnected;
	}
	return bCanRetry ? EWalletConnectedFunctionResult::FailedCanRetry : EWalletConnectedFunctionResult::Failed;
}

bool UThirdwebSubsystem::IsConnected(const FWalletHandle& Wallet, bool& bIsConnected, bool& CanRetry, FString& Error)
{
	FString Output;
	if (Thirdweb::is_connected(Wallet.ID).AssignResult(CanRetry, Output))
	{
		bIsConnected = Output.ToBool();
		return true;
	}
	Error = Output;
	return false;
}

EFunctionResult UThirdwebSubsystem::BP_Disconnect(const FWalletHandle& Wallet, FString& Error)
{
	bool bCanRetry;
	if (Disconnect(Wallet, bCanRetry, Error))
	{
		return EFunctionResult::Success;
	}
	return bCanRetry ? EFunctionResult::FailedCanRetry : EFunctionResult::Failed;
}

bool UThirdwebSubsystem::Disconnect(const FWalletHandle& Wallet, bool& CanRetry, FString& Error)
{
	FString Output;
	if (Thirdweb::disconnect(Wallet.ID).AssignResult(CanRetry, Output))
	{
		return true;
	}
	Error = Output;
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


