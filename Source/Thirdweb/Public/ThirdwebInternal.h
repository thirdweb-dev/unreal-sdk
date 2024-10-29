// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "Containers/UnrealString.h"

#include "Templates/SharedPointer.h"

struct FWalletHandle;
struct FSmartWalletHandle;
struct FInAppWalletHandle;

// Created as a struct instead of a namespaces so the functions are not exported.
struct FThirdwebAnalytics
{
	static void SendConnectEvent(const FWalletHandle Wallet);
	static void SendConnectEvent(const FInAppWalletHandle Wallet);
	static void SendConnectEvent(const FSmartWalletHandle Wallet);

	static FString GenerateUUID();
private:
	static FString GetPluginVersion();
};

