// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "ThirdwebWalletHandle.generated.h"

enum class EThirdwebOAuthProvider : uint8;
struct FSigner;

/** Unique handle that can be used to distinguish wallets */
USTRUCT(BlueprintType, Blueprintable)
struct FWalletHandle
{
	GENERATED_BODY()

	enum EWalletHandleType
	{
		InvalidHandle,
		PrivateKey,
		InApp,
		Smart
	};

	friend class UThirdwebSubsystem;
	friend class UThirdwebFunctionLibrary;

	FWalletHandle()
	{
	}

	explicit FWalletHandle(const EWalletHandleType InType, const int64 InID)
	{
		ensureAlwaysMsgf(InType != InvalidHandle, TEXT("Invalid handle type"));
		Type = InType;
		ensureAlwaysMsgf(InID > 0, TEXT("Invalid id 0"));
		ID = InID;
	}

	explicit FWalletHandle(const EWalletHandleType InType, const FString& Int64String);

	/** True if this handle was ever initialized by the thirdweb subsystem */
	bool IsValid() const { return Type != InvalidHandle && ID != 0; }

	/** Explicitly clear handle */
	void Invalidate()
	{
		ID = 0;
		Type = InvalidHandle;
	}

	/** Create a private key wallet handle directly from a private key */
	static FWalletHandle FromPrivateKey(const FString& PrivateKey);

	/** Generate a private key wallet handle */
	static FWalletHandle GeneratePrivateKey();

	/**
	 * Creates an in-app email wallet.
	 *
	 * @param Email The email address to associate with the wallet.
	 * @param Wallet An output parameter that will hold the newly created wallet handle if the creation is successful.
	 * @param Error An output parameter that will hold any error message in case of a failure.
	 * @return True if the wallet creation is successful, false otherwise.
	 */
	static bool CreateInAppEmailWallet(const FString& Email, FWalletHandle& Wallet, FString& Error);

	/**
	 * Creates an in-app OAuth wallet using the specified provider.
	 *
	 * @param Provider The OAuth provider to use for wallet creation.
	 * @param Wallet An output parameter that will hold the newly created wallet handle if the creation is successful.
	 * @param Error An output parameter that will hold any error message in case of a failure.
	 * @return True if the wallet creation is successful, false otherwise.
	 */
	static bool CreateInAppOAuthWallet(const EThirdwebOAuthProvider Provider, FWalletHandle& Wallet, FString& Error);

	/**
	 * Creates a smart wallet associated with the specified blockchain network.
	 *
	 * @param ChainID The ID of the blockchain to associate with the smart wallet.
	 * @param bGasless Specifies whether the smart wallet should be gasless or not.
	 * @param Factory The factory address used to create the smart wallet.
	 * @param AccountOverride An optional account override string.
	 * @param SmartWallet An output parameter that will hold the newly created smart wallet handle if the creation is successful.
	 * @param Error An output parameter that will hold any error message in case of a failure.
	 * @return True if the smart wallet creation is successful, false otherwise.
	 */
	bool CreateSmartWallet(const int64 ChainID,
	                       const bool bGasless,
	                       const FString& Factory,
	                       const FString& AccountOverride,
	                       FWalletHandle& SmartWallet,
	                       FString& Error);

	/** Check if the smart wallet is deployed */
	bool IsDeployed(bool& bDeployed, FString& Error);

	/** Check if the wallet is connected to a session */
	bool IsConnected() const;

	/** Disconnect a wallet from a session */
	void Disconnect() const;

	/** Get the public address of the current wallet */
	FString ToAddress() const;

	bool VerifyOTP(const FString& OTP, bool& CanRetry, FString& Error);
	bool SendOTP(FString& Error);

	bool FetchOAuthLoginURL(const FString& RedirectUrl, FString& LoginLink, FString& Error);
	bool SignInWithOAuth(const FString& AuthResult, FString& Error);

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

	/** sign a message */
	FString Sign(const FString& Message) const;

	bool operator==(const FWalletHandle& Other) const
	{
		return ID == Type == Other.Type && Other.ID;
	}

	bool operator!=(const FWalletHandle& Other) const
	{
		return Type != Other.Type || ID != Other.ID;
	}

	FString ToString() const
	{
		return IsValid()
			       ? FString::Printf(
				       TEXT("%sWallet:%lld"),
				       Type == PrivateKey
					       ? TEXT("PrivateKey")
					       : Type == InApp
					       ? TEXT("InApp")
					       : Type == Smart
					       ? TEXT("Smart")
					       : TEXT("Unknown"),
				       ID)
			       : TEXT("INVALID");
	}

private:
	// The current Handle type
	EWalletHandleType Type = InvalidHandle;

	// The wallet handle id
	UPROPERTY(Transient)
	int64 ID = 0;

	friend uint32 GetTypeHash(const FWalletHandle& InHandle)
	{
		return GetTypeHash(InHandle.ID);
	}
};
