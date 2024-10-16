// Copyright (c) 2024 Thirdweb. All Rights Reserved.

// ReSharper disable CppParameterMayBeConst,CppPassValueParameterByConstReference

#include "ThirdwebFunctionLibrary.h"

#include "Thirdweb.h"
#include "ThirdwebCommon.h"
#include "ThirdwebSigner.h"
#include "ThirdwebUtils.h"

#include "Data/ThirdwebCountryCodes.h"

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

FThirdwebCountryCode UThirdwebFunctionLibrary::BP_GetCountryCodeData(const int32 CountryCode)
{
	return ThirdwebCountryCodes::GetCountryCodeData(CountryCode);
}

TArray<FThirdwebCountryCode> UThirdwebFunctionLibrary::BP_GetAllCountryCodeData()
{
	return ThirdwebCountryCodes::GetCountryCodesArray();
}

