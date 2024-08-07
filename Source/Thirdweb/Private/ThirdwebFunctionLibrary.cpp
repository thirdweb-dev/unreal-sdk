// Copyright (c) 2024 Thirdweb. All Rights Reserved.

// ReSharper disable CppParameterMayBeConst,CppPassValueParameterByConstReference

#include "ThirdwebFunctionLibrary.h"

#include "Thirdweb.h"
#include "ThirdwebCommon.h"
#include "ThirdwebWalletHandle.h"

FWalletHandle UThirdwebFunctionLibrary::Conv_StringToWalletHandle(FString PrivateKey)
{
	return FWalletHandle::FromPrivateKey(PrivateKey);
}

bool UThirdwebFunctionLibrary::EqualEqual_WalletHandleWalletHandle(FWalletHandle A, FWalletHandle B)
{
	return A == B;
}

bool UThirdwebFunctionLibrary::NotEqual_WalletHandleWalletHandle(FWalletHandle A, FWalletHandle B)
{
	return A != B;
}

FWalletHandle UThirdwebFunctionLibrary::BP_GenerateWallet()
{
	return FWalletHandle::GeneratePrivateKey();
}

FString UThirdwebFunctionLibrary::Conv_WalletHandleToString(FWalletHandle Wallet)
{
	return Wallet.ToAddress();
}

FString UThirdwebFunctionLibrary::BP_SignMessage(const FWalletHandle& Wallet, const FString& Message)
{
	return Wallet.Sign(Message);
}

bool UThirdwebFunctionLibrary::BP_WalletIsConnected(const FWalletHandle& Wallet)
{
	return Wallet.IsConnected();
}

void UThirdwebFunctionLibrary::BP_DisconnectWallet(const FWalletHandle& Wallet)
{
	return Wallet.Disconnect();
}

EOTPVerificationFunctionResult UThirdwebFunctionLibrary::BP_VerifyOTP(FWalletHandle Wallet, const FString& OTP, FString& Error)
{
	bool bCanRetry = false;
	if (Wallet.VerifyOTP(OTP, bCanRetry, Error))
	{
		return EOTPVerificationFunctionResult::Verified;
	}
	return bCanRetry ? EOTPVerificationFunctionResult::Retry : EOTPVerificationFunctionResult::Failed;
}

EFunctionResult UThirdwebFunctionLibrary::BP_SendOTP(FWalletHandle Wallet, FString& Error)
{
	return Wallet.SendOTP(Error) ? EFunctionResult::Success : EFunctionResult::Failed;
}

EFunctionResult UThirdwebFunctionLibrary::BP_FetchOAuthLoginLink(FWalletHandle Wallet, const FString& RedirectUrl, FString& LoginLink, FString& Error)
{
	return Wallet.FetchOAuthLoginURL(RedirectUrl, LoginLink, Error) ? EFunctionResult::Success : EFunctionResult::Failed;
}

bool UThirdwebFunctionLibrary::BP_WalletIsValid(const FWalletHandle& Wallet)
{
	return Wallet.IsValid();
}

ESmartWalletDeployedFunctionResult UThirdwebFunctionLibrary::BP_IsSmartWalletDeployed(FWalletHandle Wallet, FString& Error)
{
	if (bool bDeployed; Wallet.IsDeployed(bDeployed, Error))
	{
		return bDeployed ? ESmartWalletDeployedFunctionResult::Deployed : ESmartWalletDeployedFunctionResult::NotDeployed;
	}
	return ESmartWalletDeployedFunctionResult::Failed;
}

EFunctionResult UThirdwebFunctionLibrary::BP_CreateSmartWalletSessionKey(FWalletHandle Wallet, const FString& Signer, const TArray<FString>& ApprovedTargets,
                                                                         const FString& NativeTokenLimitPerTransactionInWei, const FDateTime& PermissionStart, const FDateTime& PermissionEnd,
                                                                         const FDateTime& RequestValidityStart, const FDateTime& RequestValidityEnd,
                                                                         FString& TransactionHash, FString& Error)
{
	return Wallet.CreateSessionKey(Signer, ApprovedTargets, NativeTokenLimitPerTransactionInWei, PermissionStart, PermissionEnd, RequestValidityStart, RequestValidityEnd, TransactionHash, Error)
		       ? EFunctionResult::Success
		       : EFunctionResult::Failed;
}

EFunctionResult UThirdwebFunctionLibrary::BP_GetSmartWalletAdmins(FWalletHandle Wallet, TArray<FString>& Admins, FString& Error)
{
	return Wallet.GetAdmins(Admins, Error) ? EFunctionResult::Success : EFunctionResult::Failed;
}

EFunctionResult UThirdwebFunctionLibrary::BP_GetSmartWalletActiveSigners(FWalletHandle Wallet, TArray<FSigner>& Signers, FString& Error)
{
	return Wallet.GetActiveSigners(Signers, Error) ? EFunctionResult::Success : EFunctionResult::Failed;
}

EFunctionResult UThirdwebFunctionLibrary::BP_RevokeSmartWalletSessionKey(FWalletHandle Wallet, const FString& Signer, FString& Error)
{
	return Wallet.RevokeSessionKey(Signer, Error) ? EFunctionResult::Success : EFunctionResult::Failed;
}

EFunctionResult UThirdwebFunctionLibrary::BP_AddSmartWalletAdmin(FWalletHandle Wallet, const FString& Signer, FString& Error)
{
	return Wallet.AddAdmin(Signer, Error) ? EFunctionResult::Success : EFunctionResult::Failed;
}

EFunctionResult UThirdwebFunctionLibrary::BP_RemoveSmartWalletAdmin(FWalletHandle Wallet, const FString& Signer, FString& Error)
{
	return Wallet.RemoveAdmin(Signer, Error) ? EFunctionResult::Success : EFunctionResult::Failed;
}

FText UThirdwebFunctionLibrary::Conv_ThirdwebOAuthProviderToText(EThirdwebOAuthProvider Provider)
{
	return ToText(Provider);
}

FString UThirdwebFunctionLibrary::Conv_ThirdwebOAuthProviderToString(EThirdwebOAuthProvider Provider)
{
	return ToString(Provider);
}

bool UThirdwebFunctionLibrary::BP_IsStringValidAddress(const FString& Address, const bool bWithChecksum)
{
	return Thirdweb::IsValidAddress(Address, bWithChecksum);
}

bool UThirdwebFunctionLibrary::BP_IsStringChecksummedAddress(const FString& Address)
{
	return Thirdweb::IsChecksummedAddress(Address);
}

bool UThirdwebFunctionLibrary::BP_IsStringValidPrivateKey(const FString& PrivateKey)
{
	return Thirdweb::IsValidPrivateKey(PrivateKey);
}

FString UThirdwebFunctionLibrary::Conv_StringAddressToStringChecksummedAddress(const FString& Address)
{
	return Thirdweb::ToChecksummedAddress(Address);
}

bool UThirdwebFunctionLibrary::BP_IsTextValidAddress(const FText& Address, const bool bWithChecksum)
{
	return !Address.IsEmpty() && Thirdweb::IsValidAddress(Address.ToString(), bWithChecksum);
}

bool UThirdwebFunctionLibrary::BP_IsTextChecksummedAddress(const FText& Address)
{
	return !Address.IsEmpty() && Thirdweb::IsChecksummedAddress(Address.ToString());
}

bool UThirdwebFunctionLibrary::BP_IsTextValidPrivateKey(const FText& PrivateKey)
{
	return !PrivateKey.IsEmpty() && Thirdweb::IsValidPrivateKey(PrivateKey.ToString());
}

FText UThirdwebFunctionLibrary::Conv_TextAddressToStringChecksummedAddress(const FText& Address)
{
	return Address.IsEmpty() ? FText::GetEmpty() : FText::FromString(Thirdweb::ToChecksummedAddress(Address.ToString()));
}
