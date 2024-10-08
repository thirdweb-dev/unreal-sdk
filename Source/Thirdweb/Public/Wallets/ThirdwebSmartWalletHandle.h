// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "ThirdwebInAppWalletHandle.h"
#include "ThirdwebWalletHandle.h"
#include "ThirdwebSmartWalletHandle.generated.h"

struct FSigner;

USTRUCT(BlueprintType, Blueprintable)
struct THIRDWEB_API FSmartWalletHandle : public FWalletHandle
{
	GENERATED_BODY()

	DECLARE_DELEGATE_OneParam(FCreateSmartWalletDelegate, FSmartWalletHandle);
	DECLARE_DELEGATE_OneParam(FGetActiveSignersDelegate, const TArray<FSigner>&);
	
	FSmartWalletHandle()
	{
		Type = Smart;
	}

private:
	explicit FSmartWalletHandle(const FInAppWalletHandle& InInAppWallet, const int64 InID)
	{
		Type = Smart;
		InAppWallet = InInAppWallet;
		ensureAlwaysMsgf(InID > 0, TEXT("Invalid id 0"));
		ID = InID;
	}

	explicit FSmartWalletHandle(const FInAppWalletHandle& InInAppWallet, const FString& Int64String);

public:
	virtual bool IsValid() const override { return Super::IsValid() && InAppWallet.IsValid(); }

	/** Explicitly clear handle */
	virtual void Invalidate() override
	{
		Super::Invalidate();
		InAppWallet.Invalidate();
	}

	/**
	 * Creates a new FSmartWalletHandle instance.
	 *
	 * @param ChainID The ID of the blockchain to connect to.
	 * @param InInAppWallet The InApp Wallet signer.
	 * @param bGasless A boolean indicating if the wallet should operate without gas fees.
	 * @param Factory The factory contract address for wallet creation.
	 * @param AccountOverride Optional account address to override the default.
	 * @param bSuccess Output parameter that will be set to true if the wallet creation is successful, false otherwise.
	 * @param Error Output parameter that will hold the error message if the creation fails.
	 * @return A new instance of FSmartWalletHandle.
	 */
	static void Create(const FInAppWalletHandle& InInAppWallet, const int64 ChainID, const bool bGasless, const FString& Factory, const FString& AccountOverride, const FCreateSmartWalletDelegate& SuccessDelegate, const FStringDelegate& ErrorDelegate);

	/** Check if the smart wallet is deployed */
	void IsDeployed(const FBoolDelegate& SuccessDelegate, const FStringDelegate& ErrorDelegate);
	
	/** Create a session key for a smart wallet */
	void CreateSessionKey(
		const FString& Signer,
		const TArray<FString>& ApprovedTargets,
		const FString& NativeTokenLimitPerTransactionInWei,
		const FDateTime& PermissionStart,
		const FDateTime& PermissionEnd,
		const FDateTime& RequestValidityStart,
		const FDateTime& RequestValidityEnd,
		const FStringDelegate& SuccessDelegate,
		const FStringDelegate& ErrorDelegate
	);

	/** Revoke a session key for a smart wallet */
	void RevokeSessionKey(const FString& Signer, const FSimpleDelegate& SuccessDelegate, const FStringDelegate& ErrorDelegate);

	/** Get the admins of a smart wallet */
	void GetAdmins(const FStringArrayDelegate& SuccessDelegate, const FStringDelegate& ErrorDelegate);

	/** Add an admin to a smart wallet */
	void AddAdmin(const FString& Signer, const FSimpleDelegate& SuccessDelegate, const FStringDelegate& ErrorDelegate);

	/** Remove an admin from a smart wallet */
	void RemoveAdmin(const FString& Signer, const FSimpleDelegate& SuccessDelegate, const FStringDelegate& ErrorDelegate);

	/** Get the active signers of a smart wallet */
	void GetActiveSigners(const FGetActiveSignersDelegate& SuccessDelegate, const FStringDelegate& ErrorDelegate);

private:
	FInAppWalletHandle InAppWallet;
};
