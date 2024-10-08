﻿// Copyright (c) 2024 Thirdweb. All Rights Reserved.

// ReSharper disable CppParameterMayBeConst,CppPassValueParameterByConstReference

#include "ThirdwebFunctionLibrary.h"

#include "Thirdweb.h"
#include "ThirdwebCommon.h"
#include "ThirdwebSigner.h"
#include "ThirdwebUtils.h"

#include "Wallets/ThirdwebInAppWalletHandle.h"
#include "Wallets/ThirdwebSmartWalletHandle.h"
#include "Wallets/ThirdwebWalletHandle.h"

bool UThirdwebFunctionLibrary::EqualEqual_InAppWalletHandleInAppWalletHandle(FInAppWalletHandle A, FInAppWalletHandle B)
{
	return A == B;
}

bool UThirdwebFunctionLibrary::NotEqual_InAppWalletHandleInAppWalletHandle(FInAppWalletHandle A, FInAppWalletHandle B)
{
	return A != B;
}

bool UThirdwebFunctionLibrary::EqualEqual_SmartWalletHandleSmartWalletHandle(FSmartWalletHandle A, FSmartWalletHandle B)
{
	return A == B;
}

bool UThirdwebFunctionLibrary::NotEqual_SmartWalletHandleSmartWalletHandle(FSmartWalletHandle A, FSmartWalletHandle B)
{
	return A != B;
}

FString UThirdwebFunctionLibrary::Conv_InAppWalletHandleToString(FInAppWalletHandle Wallet)
{
	return Wallet.ToAddress();
}

FString UThirdwebFunctionLibrary::Conv_SmartWalletHandleToString(FSmartWalletHandle Wallet)
{
	return Wallet.ToAddress();
}

FText UThirdwebFunctionLibrary::Conv_InAppWalletHandleToText(FInAppWalletHandle Wallet)
{
	return FText::FromString(Conv_InAppWalletHandleToString(Wallet));
}

FText UThirdwebFunctionLibrary::Conv_SmartWalletHandleToText(FSmartWalletHandle Wallet)
{
	return FText::FromString(Conv_SmartWalletHandleToString(Wallet));
}

EFunctionResult UThirdwebFunctionLibrary::BP_CreateInAppEmailWallet(const FString& Email, const FString& PartnerId, FInAppWalletHandle& Wallet, FString& Error)
{
	return
	// (UThirdwebRuntimeSettings::IsEcosystem()
	// 	        ? FInAppWalletHandle::CreateEcosystemEmailWallet(PartnerId, Email, Wallet, Error)
	// 	        : FInAppWalletHandle::CreateEmailWallet(Email, Wallet, Error))
	// 	       ? EFunctionResult::Success
	// 	       :
	EFunctionResult::Failed;
}

EFunctionResult UThirdwebFunctionLibrary::BP_CreateInAppOAuthWallet(const EThirdwebOAuthProvider Provider, const FString& PartnerId, FInAppWalletHandle& Wallet, FString& Error)
{
	return
	// (UThirdwebRuntimeSettings::IsEcosystem()
	// 	        ? FInAppWalletHandle::CreateEcosystemOAuthWallet(PartnerId, Provider, Wallet, Error)
	// 	        : FInAppWalletHandle::CreateOAuthWallet(Provider, Wallet, Error))
	// 	       ? EFunctionResult::Success
	// 	       :
	EFunctionResult::Failed;
}

EFunctionResult UThirdwebFunctionLibrary::BP_CreateInAppPhoneWallet(const FString& Phone, const FString& PartnerId, FInAppWalletHandle& Wallet, FString& Error)
{
	return
	// (UThirdwebRuntimeSettings::IsEcosystem()
	// 	        ? FInAppWalletHandle::CreateEcosystemPhoneWallet(PartnerId, Phone, Wallet, Error)
	// 	        : FInAppWalletHandle::CreatePhoneWallet(Phone, Wallet, Error))
	// 	       ? EFunctionResult::Success
	// 	       :
	EFunctionResult::Failed;
}

EFunctionResult UThirdwebFunctionLibrary::BP_CreateInAppJwtWallet(const FString& PartnerId, FInAppWalletHandle& Wallet, FString& Error)
{
	return
	// (UThirdwebRuntimeSettings::IsEcosystem()
	// 	        ? FInAppWalletHandle::CreateEcosystemJwtWallet(PartnerId, Wallet, Error)
	// 	        : FInAppWalletHandle::CreateJwtWallet(Wallet, Error))
	// 	       ? EFunctionResult::Success
	// 	       :
	EFunctionResult::Failed;
}

EFunctionResult UThirdwebFunctionLibrary::BP_CreateInAppAuthEndpointWallet(const FString& PartnerId, FInAppWalletHandle& Wallet, FString& Error)
{
	return
	// (UThirdwebRuntimeSettings::IsEcosystem()
	// 	        ? FInAppWalletHandle::CreateEcosystemAuthEndpointWallet(PartnerId, Wallet, Error)
	// 	        : FInAppWalletHandle::CreateAuthEndpointWallet(Wallet, Error))
	// 	       ? EFunctionResult::Success
	// 	       :
	EFunctionResult::Failed;
}

EFunctionResult UThirdwebFunctionLibrary::BP_CreateInAppGuestWallet(const FString& PartnerId, FInAppWalletHandle& Wallet, FString& Error)
{
	return
	// (UThirdwebRuntimeSettings::IsEcosystem()
	// 	        ? FInAppWalletHandle::CreateEcosystemGuestWallet(PartnerId, Wallet, Error)
	// 	        : FInAppWalletHandle::CreateGuestWallet(Wallet, Error))
	// 	       ? EFunctionResult::Success
	// 	       :
	EFunctionResult::Failed;
}

EFunctionResult UThirdwebFunctionLibrary::BP_CreateSmartWallet(const FInAppWalletHandle PersonalWallet,
                                                               FSmartWalletHandle& SmartWallet,
                                                               FString& Error,
                                                               const int64 ChainID,
                                                               const bool bGasless,
                                                               const FString& Factory,
                                                               const FString& AccountOverride)
{
	// bool bSuccessful;
	// FSmartWalletHandle SmartWalletHandle = FSmartWalletHandle::Create(PersonalWallet, ChainID, bGasless, Factory, AccountOverride, bSuccessful, Error);
	return
	// bSuccessful ? EFunctionResult::Success :
	EFunctionResult::Failed;
}

FString UThirdwebFunctionLibrary::BP_SignInAppMessage(const FInAppWalletHandle& Wallet, const FString& Message)
{
	return ""; // Wallet.Sign(Message);
}

FString UThirdwebFunctionLibrary::BP_SignSmartMessage(const FSmartWalletHandle& Wallet, const FString& Message)
{
	return ""; // Wallet.Sign(Message);
}

bool UThirdwebFunctionLibrary::BP_WalletIsConnected(const FInAppWalletHandle& Wallet)
{
	return Wallet.IsConnected();
}

void UThirdwebFunctionLibrary::BP_DisconnectWallet(const FInAppWalletHandle& Wallet)
{
	return Wallet.Disconnect();
}


EFunctionResult UThirdwebFunctionLibrary::BP_FetchOAuthLoginLink(FInAppWalletHandle Wallet, const FString& RedirectUrl, FString& LoginLink, FString& Error)
{
	return Wallet.FetchOAuthLoginURL(RedirectUrl, LoginLink, Error) ? EFunctionResult::Success : EFunctionResult::Failed;
}

EFunctionResult UThirdwebFunctionLibrary::BP_SignInWithOAuth(FInAppWalletHandle Wallet, const FString& AuthResult, FString& Error)
{
	return /** Wallet.SignInWithOAuth(AuthResult, Error) ? EFunctionResult::Success : */ EFunctionResult::Failed;
}

EFunctionResult UThirdwebFunctionLibrary::BP_SignInWithJwt(FInAppWalletHandle Wallet, const FString& Jwt, FString& Error)
{
	return /** Wallet.SignInWithJwt(Jwt, Error) ? EFunctionResult::Success : */ EFunctionResult::Failed;
}

EFunctionResult UThirdwebFunctionLibrary::BP_SignInWithAuthEndpoint(FInAppWalletHandle Wallet, const FString& Payload, FString& Error)
{
	return /** Wallet.SignInWithAuthEndpoint(Payload, Error) ? EFunctionResult::Success : */ EFunctionResult::Failed;
}

EFunctionResult UThirdwebFunctionLibrary::BP_SignInWithGuest(FInAppWalletHandle Wallet, FString& Error)
{
	return /** Wallet.SignInWithGuest(Error) ? EFunctionResult::Success : */ EFunctionResult::Failed;
}

bool UThirdwebFunctionLibrary::BP_InAppWalletIsValid(const FInAppWalletHandle& Wallet)
{
	return Wallet.IsValid();
}

bool UThirdwebFunctionLibrary::BP_SmartWalletIsValid(const FSmartWalletHandle& Wallet)
{
	return Wallet.IsValid();
}

FText UThirdwebFunctionLibrary::Conv_ThirdwebOAuthProviderToText(EThirdwebOAuthProvider Provider)
{
	return ThirdwebUtils::ToText(Provider);
}

FString UThirdwebFunctionLibrary::Conv_ThirdwebOAuthProviderToString(EThirdwebOAuthProvider Provider)
{
	return ThirdwebUtils::ToString(Provider);
}

EThirdwebOAuthProvider UThirdwebFunctionLibrary::Conv_TextToThirdwebOAuthProvider(FText Text)
{
	return ThirdwebUtils::ToOAuthProvider(Text);
}

EThirdwebOAuthProvider UThirdwebFunctionLibrary::Conv_StringToThirdwebOAuthProvider(FString String)
{
	return ThirdwebUtils::ToOAuthProvider(String);
}

bool UThirdwebFunctionLibrary::BP_IsStringValidAddress(const FString& Address, const bool bWithChecksum)
{
	return ThirdwebUtils::IsValidAddress(Address, bWithChecksum);
}

bool UThirdwebFunctionLibrary::BP_IsStringChecksummedAddress(const FString& Address)
{
	return ThirdwebUtils::IsChecksummedAddress(Address);
}

FString UThirdwebFunctionLibrary::Conv_StringAddressToStringChecksummedAddress(const FString& Address)
{
	return ThirdwebUtils::ToChecksummedAddress(Address);
}

bool UThirdwebFunctionLibrary::BP_IsTextValidAddress(const FText Address, const bool bWithChecksum)
{
	return !Address.IsEmpty() && ThirdwebUtils::IsValidAddress(Address.ToString(), bWithChecksum);
}

bool UThirdwebFunctionLibrary::BP_IsTextChecksummedAddress(const FText Address)
{
	return !Address.IsEmpty() && ThirdwebUtils::IsChecksummedAddress(Address.ToString());
}

FText UThirdwebFunctionLibrary::Conv_TextAddressToStringChecksummedAddress(const FText& Address)
{
	return Address.IsEmpty() ? FText::GetEmpty() : FText::FromString(ThirdwebUtils::ToChecksummedAddress(Address.ToString()));
}

FString UThirdwebFunctionLibrary::BP_ZeroAddress()
{
	return ThirdwebUtils::ZeroAddress;
}

