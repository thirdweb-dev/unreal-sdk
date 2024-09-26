// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "ThirdwebWalletHandle.generated.h"

USTRUCT()
struct THIRDWEB_API FWalletHandle
{
	GENERATED_BODY()

	virtual ~FWalletHandle() = default;

	enum EWalletHandleType
	{
		InvalidHandle,
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

	virtual FString GetDisplayName() const
	{
		return IsValid() ? FString::Printf(TEXT("%sWallet::%lld"), GetTypeString(), ID) : TEXT("INVALID");
	}

	const TCHAR* GetTypeString() const
	{
		return Type == InApp ? TEXT("inApp") : Type == Smart ? TEXT("smart") : TEXT("invalid");
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
