// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "ThirdwebMacros.h"
#include "ThirdwebWalletHandle.generated.h"

#define CHECK_DELEGATES(SuccessDelegate, ErrorDelegate) \
	if (!SuccessDelegate.IsBound()) \
	{ \
		if (ErrorDelegate.IsBound()) \
		{ \
			TW_LOG(Error, TEXT("No SuccessDelegate bound::%s"), UE_SOURCE_LOCATION) \
			ErrorDelegate.Execute(TEXT("Success Delegate Not Bound")); \
		} \
		return; \
	} \
	if (!ErrorDelegate.IsBound()) { \
		TW_LOG(Error, TEXT("No ErrorDelegate bound::%s"), UE_SOURCE_LOCATION) \
		return; \
	}

#define CHECK_VALIDITY(ErrorDelegate) \
	if (!IsValid()) \
	{ \
		TW_LOG(Error, TEXT("Invalid wallet handle")) \
		if (ErrorDelegate.IsBound()) \
		{ \
			ErrorDelegate.Execute(TEXT("Invalid wallet handle")); \
		} \
		return; \
	}

struct FSmartWalletHandle;
struct FInAppWalletHandle;

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

	explicit FWalletHandle(const FInAppWalletHandle& InAppWalletHandle);
	explicit FWalletHandle(const FSmartWalletHandle& SmartWalletHandle);
	
	/**
	 * Checks if the wallet handle is valid.
	 *
	 * @return True if the wallet handle is valid, i.e., its type is not invalid and its ID is not zero.
	 */
	virtual bool IsValid() const { return Type != InvalidHandle && ID != 0; }

	/**
	 * Invalidates the wallet handle, setting its ID to zero and marking its type as invalid.
	 */
	virtual void Invalidate()
	{
		ID = 0;
		Type = InvalidHandle;
	}

	/**
	 * Retrieves the wallet address associated with this handle.
	 *
	 * @return The wallet address as a string. Returns a zero address if the handle is invalid.
	 */
	virtual FString ToAddress() const;

	/**
	 * Signs a given message using the wallet handle.
	 *
	 * @param Message The message to be signed.
	 * @param SuccessDelegate Delegate that gets called upon a successful signing operation.
	 * @param ErrorDelegate Delegate that gets called if an error occurs during the signing operation.
	 */
	virtual void Sign(const FString& Message, const FStringDelegate& SuccessDelegate, const FStringDelegate& ErrorDelegate) const;

	/**
	 * Get the type of wallet handle.
	 * @return The type of the current wallet handle.
	 */
	EWalletHandleType GetType() const { return Type; }

	/**
	 * Gets the ID of the wallet handle.
	 *
	 * @return The ID of the wallet handle as an int64.
	 */
	int64 GetID() const { return ID; }

	/**
	 * Retrieves the debug display name of the wallet handle.
	 *
	 * @return A string representing the display name of the wallet handle.
	 * If the wallet handle is valid, the name includes the type of the wallet and its ID.
	 * Returns "INVALID" if the wallet handle is not valid.
	 */
	virtual FString GetDisplayName() const
	{
		return IsValid() ? FString::Printf(TEXT("%sWallet::%lld"), GetTypeString(), ID) : TEXT("INVALID");
	}

	/**
	 * Retrieves the type of the wallet handle as a string.
	 *
	 * @return A string representing the type of the wallet handle. Possible values are "inApp", "smart", or "invalid".
	 */
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
		return GetTypeHash(InHandle.GetID());
	}
};
