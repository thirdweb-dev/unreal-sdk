// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

// Created as a struct instead of a namespaces so the functions are not exported.
struct FThirdwebAnalytics
{

	static void SendConnectEvent(const FString& Wallet, const FString& Type);

private:
	static FString JsonObjectToString(const TSharedPtr<class FJsonObject>& JsonObject);
	static FString GetPluginVersion();
};

