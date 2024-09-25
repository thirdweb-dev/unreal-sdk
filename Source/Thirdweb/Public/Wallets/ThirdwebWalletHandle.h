// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "ThirdwebWalletHandle.generated.h"

enum class EThirdwebOTPMethod : uint8;
enum class EThirdwebOAuthProvider : uint8;
struct FSigner;

struct THIRDWEB_API FWalletHandle
{
	virtual ~FWalletHandle() = default;

	enum EWalletHandleType
	{
		InvalidHandle,
		PrivateKey,
		InApp,
		Smart
	};

	FWalletHandle()
	{
	}

	/** True if this handle was ever initialized */
	virtual bool IsValid() const { return Type != InvalidHandle && ID != 0; }

	/** Explicitly clear handle */
	virtual void Invalidate()
	{
		ID = 0;
		Type = InvalidHandle;
	}

	/** Get the public address of the current wallet */
	virtual FString ToAddress() const;

	/** sign a message */
	virtual FString Sign(const FString& Message) const;

	EWalletHandleType GetType() const { return Type; }

	int64 GetID() const { return ID; }

	virtual FString GetDisplayName() const { return IsValid() ? FString::Printf(TEXT("%sWallet::%lld"), GetTypeString(), ID) : TEXT("INVALID"); }

	const TCHAR* GetTypeString() const
	{
		return IsValid() ? Type == PrivateKey ? TEXT("privateKey") : Type == InApp ? TEXT("inApp") : Type == Smart ? TEXT("smart") : TEXT("unknown") : TEXT("invalid");
	}

	bool operator==(const FWalletHandle& Other) const
	{
		return GetID() == Other.GetID();
	}

	bool operator!=(const FWalletHandle& Other) const
	{
		return GetID() != Other.GetID();
	}

protected:
	// The current Handle type
	EWalletHandleType Type = InvalidHandle;

	// The wallet handle id
	int64 ID = 0;

	friend uint32 GetTypeHash(const FWalletHandle& InHandle)
	{
		return GetTypeHash(InHandle.ID);
	}
};

USTRUCT(BlueprintType, Blueprintable)
struct THIRDWEB_API FPrivateKeyWalletHandle : public FWalletHandle
{
	GENERATED_BODY()

	FPrivateKeyWalletHandle()
	{
		Type = PrivateKey;
	}

private:
	explicit FPrivateKeyWalletHandle(const int64 InID)
	{
		Type = PrivateKey;
		ensureAlwaysMsgf(InID > 0, TEXT("Invalid id 0"));
		ID = InID;
	}

	explicit FPrivateKeyWalletHandle(const FString& Int64String);

public:
	/** Create a private key wallet handle directly from a private key */
	static FWalletHandle FromPrivateKey(const FString& PrivateKey);

	/** Generate a private key wallet handle */
	static FWalletHandle GeneratePrivateKey();

	/** Get the private key of this handle (PrivateKey wallet only) */
	FString GetPrivateKey() const;
};

USTRUCT(BlueprintType, Blueprintable)
struct THIRDWEB_API FInAppWalletHandle : public FWalletHandle
{
	GENERATED_BODY()

	enum EInAppSource
	{
		InvalidSource,
		OAuthProvider,
		Email,
		Phone
	};

	FInAppWalletHandle()
	{
		Type = InApp;
	}

private:
	explicit FInAppWalletHandle(const EInAppSource InSource, const int64 InID)
	{
		Type = InApp;
		ensureAlwaysMsgf(InSource != InvalidSource, TEXT("Invalid source"));
		Source = InSource;
		ensureAlwaysMsgf(InID > 0, TEXT("Invalid id 0"));
		ID = InID;
	}

	explicit FInAppWalletHandle(const EInAppSource InType, const FString& Int64String);

public:
	virtual bool IsValid() const override { return Super::IsValid() && Source != InvalidSource; }

	/** Explicitly clear handle */
	virtual void Invalidate() override
	{
		Super::Invalidate();
		Source = InvalidSource;
	}
	
	/**
	 * Creates an in-app email wallet.
	 *
	 * @param Email The email address to associate with the wallet.
	 * @param Wallet An output parameter that will hold the newly created wallet handle if the creation is successful.
	 * @param Error An output parameter that will hold any error message in case of a failure.
	 * @return True if the wallet creation is successful, false otherwise.
	 */
	static bool CreateEmailWallet(const FString& Email, FWalletHandle& Wallet, FString& Error);

	/**
	 * Creates an in-app OAuth wallet using the specified provider.
	 *
	 * @param Provider The OAuth provider to use for wallet creation.
	 * @param Wallet An output parameter that will hold the newly created wallet handle if the creation is successful.
	 * @param Error An output parameter that will hold any error message in case of a failure.
	 * @return True if the wallet creation is successful, false otherwise.
	 */
	static bool CreateOAuthWallet(const EThirdwebOAuthProvider Provider, FWalletHandle& Wallet, FString& Error);

	/**
	 * Creates an in-app phone wallet.
	 *
	 * @param Phone The phone number to associate with the wallet.
	 * @param Wallet An output parameter that will hold the newly created wallet handle if the creation is successful.
	 * @param Error An output parameter that will hold any error message in case of a failure.
	 * @return True if the wallet creation is successful, false otherwise.
	 */
	static bool CreatePhoneWallet(const FString& Phone, FWalletHandle& Wallet, FString& Error);
	
	/** Check if the wallet is connected to a session */
	bool IsConnected() const;

	/** Disconnect a wallet from a session */
	void Disconnect() const;

	bool VerifyOTP(const EThirdwebOTPMethod Method, const FString& OTP, FString& Error);
	bool SendOTP(const EThirdwebOTPMethod Method, FString& Error);

	bool FetchOAuthLoginURL(const FString& RedirectUrl, FString& LoginLink, FString& Error);
	bool SignInWithOAuth(const FString& AuthResult, FString& Error);
	bool SignInWithJwt(const FString& Jwt, const FString& EncryptionKey, FString& Error);
	bool SignInWithEndpoint(const FString& Payload, const FString& EncryptionKey, FString& Error);
	bool SignInWithGuest(const FString& SessionId, FString& Error);

    const TCHAR* GetSourceString() const { return  Source == Email ? TEXT("Email") : Source == Phone ? TEXT("Phone") : Source == OAuthProvider ? TEXT("OAuthProvider") : TEXT("Invalid"); }

	virtual FString GetDisplayName() const override { return IsValid() ? FString::Printf(TEXT("%sWallet::%s::%lld"), GetTypeString(), GetSourceString(), ID) : TEXT("InAppWallet::INVALID"); }


private:
	// The current Handle type
	EInAppSource Source = InvalidSource;
};

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
	 * @param bGasless A boolean indicating if the wallet should operate without gas fees.
	 * @param Factory The factory contract address for wallet creation.
	 * @param AccountOverride Optional account address to override the default.
	 * @param bSuccess Output parameter that will be set to true if the wallet creation is successful, false otherwise.
	 * @param Error Output parameter that will hold the error message if the creation fails.
	 * @return A new instance of FSmartWalletHandle.
	 */
	static FSmartWalletHandle Create(const int64 ChainID, const bool bGasless, const FString& Factory, const FString& AccountOverride, bool bSuccess, FString& Error);

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
