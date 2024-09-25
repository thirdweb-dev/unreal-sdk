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
	static FSmartWalletHandle Create(const FInAppWalletHandle& InInAppWallet, const int64 ChainID, const bool bGasless, const FString& Factory, const FString& AccountOverride, bool& bSuccess, FString& Error);

	/** Check if the smart wallet is deployed */
	bool IsDeployed(bool& bDeployed, FString& Error);
	
	/** Create a session key for a smart wallet */
	bool CreateSessionKey(
		const FString& Signer,
		const TArray<FString>& ApprovedTargets,
		const FString& NativeTokenLimitPerTransactionInWei,
		const FDateTime& PermissionStart,
		const FDateTime& PermissionEnd,
		const FDateTime& RequestValidityStart,
		const FDateTime& RequestValidityEnd,
		FString& TransactionHash,
		FString& Error
	);

	/** Revoke a session key for a smart wallet */
	bool RevokeSessionKey(const FString& Signer, FString& Error);

	/** Get the admins of a smart wallet */
	bool GetAdmins(TArray<FString>& Admins, FString& Error);

	/** Add an admin to a smart wallet */
	bool AddAdmin(const FString& Signer, FString& Error);

	/** Remove an admin from a smart wallet */
	bool RemoveAdmin(const FString& Signer, FString& Error);

	/** Get the active signers of a smart wallet */
	bool GetActiveSigners(TArray<FSigner>& Signers, FString& Error);

private:
	FInAppWalletHandle InAppWallet;
};
