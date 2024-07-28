// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "ThirdwebWalletHandle.generated.h"

/** Unique handle that can be used to distinguish wallets */
USTRUCT(BlueprintType)
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
	
	FWalletHandle() {}

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
	
	/** Create a private key wallet handle directly from a private key **/
	static FWalletHandle FromPrivateKey(const FString& PrivateKey);

	/** Generate a private key wallet handle **/
	static FWalletHandle GeneratePrivateKey();

	/** Check if the smart wallet is deployed */
	bool IsDeployed(bool& bDeployed, FString& Error);
	
	/** Check if the wallet is connected to a session */
	bool IsConnected() const;
	
	/** Disconnect a wallet from a session */
	void Disconnect() const;

	/** Get the public address of the current wallet **/
	FString ToAddress() const;

	bool VerifyOTP(const FString& OTP, bool& CanRetry, FString& Error);
	bool SendOTP(FString& Error);

	bool FetchOAuthLoginURL(const FString& RedirectUrl, FString& LoginLink, FString& Error);
	bool SignInWithOAuth(const FString& AuthResult, FString& Error);
	
	/** 
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
