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
	
	FWalletHandle()
		: Type(InvalidHandle), ID(0)
	{
	}

	explicit FWalletHandle(const EWalletHandleType InType, const int64 InID)
		: Type(InType), ID(InID)
	{
	}
	explicit FWalletHandle(const EWalletHandleType InType, const FString& InID);
	
	/** True if this handle was ever initialized by the thirdweb subsystem */
	bool IsValid() const
	{
		return Type != InvalidHandle && ID != 0;
	}

	/** Explicitly clear handle */
	void Invalidate()
	{
		ID = 0;
		Type = InvalidHandle;
	}

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

	// Invalid wallet handle definition
	static FWalletHandle Invalid;
private:
	// The current Handle type
	EWalletHandleType Type;

	// The wallet handle id
	UPROPERTY(Transient)
	int64 ID;


	friend uint32 GetTypeHash(const FWalletHandle& InHandle)
	{
		return GetTypeHash(InHandle.ID);
	}
};
