#include "ThirdwebInternal.h"

#include <random>

#include "HttpModule.h"
#include "ThirdwebRuntimeSettings.h"
#include "ThirdwebUtils.h"

#include "Dom/JsonObject.h"

#include "Interfaces/IPluginManager.h"

#include "Kismet/GameplayStatics.h"

#include "Wallets/ThirdwebInAppWalletHandle.h"
#include "Wallets/ThirdwebSmartWalletHandle.h"
#include "Wallets/ThirdwebWalletHandle.h"


FString FThirdwebAnalytics::GetPluginVersion()
{
	if (const TSharedPtr<IPlugin> Plugin = IPluginManager::Get().FindPlugin(TEXT("Thirdweb")); Plugin.IsValid())
	{
		return Plugin->GetDescriptor().VersionName;
	}
	return "0.0.0";
}

// ReSharper disable CppPassValueParameterByConstReference
void FThirdwebAnalytics::SendConnectEvent(const FWalletHandle Wallet)
{
	if (!IsInGameThread())
	{
		// Retry on the GameThread.
		const FWalletHandle WalletCopy;
		FFunctionGraphTask::CreateAndDispatchWhenReady([WalletCopy]()
		{
			SendConnectEvent(WalletCopy);
		}, TStatId(), nullptr, ENamedThreads::GameThread);
		return;
	}
	const UThirdwebRuntimeSettings* Settings = UThirdwebRuntimeSettings::Get();
	if (!UThirdwebRuntimeSettings::AnalyticsEnabled() || UThirdwebRuntimeSettings::GetBundleId().IsEmpty() || UThirdwebRuntimeSettings::GetClientId().IsEmpty())
	{
		return;
	}
	FHttpModule& HttpModule = FHttpModule::Get();
	const TSharedRef<IHttpRequest> Request = HttpModule.CreateRequest();
	Request->SetVerb("POST");
	Request->SetURL("https://c.thirdweb.com/event");
	Request->SetHeader("Content-Type", "application/json");
	Request->SetHeader("x-sdk-name", "UnrealEngineSDK");
	Request->SetHeader("x-sdk-os", UGameplayStatics::GetPlatformName());
	Request->SetHeader("x-sdk-platform", "unreal-engine");
	Request->SetHeader("x-sdk-version", GetPluginVersion());
	Request->SetHeader("x-client-id", Settings->ClientID);
	Request->SetHeader("x-bundle-id", Settings->BundleID);
	Request->SetTimeout(5.0f);

	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	JsonObject->SetStringField(TEXT("source"), TEXT("connectWallet"));
	JsonObject->SetStringField(TEXT("action"), TEXT("connect"));
	JsonObject->SetStringField(TEXT("walletAddress"), Wallet.ToAddress());
	JsonObject->SetStringField(TEXT("walletType"), Wallet.GetTypeString());
	Request->SetContentAsString(ThirdwebUtils::Json::ToString(JsonObject));
	Request->ProcessRequest();
}

void FThirdwebAnalytics::SendConnectEvent(const FInAppWalletHandle Wallet) { SendConnectEvent(FWalletHandle(Wallet)); }

void FThirdwebAnalytics::SendConnectEvent(const FSmartWalletHandle Wallet) { SendConnectEvent(FWalletHandle(Wallet)); }
// ReSharper restore CppPassValueParameterByConstReference

// https://stackoverflow.com/a/58467162/12204515
FString FThirdwebAnalytics::GenerateUUID()
{
	static std::random_device Device;
	static std::mt19937 RNG(Device());

	std::uniform_int_distribution Distribution(0, 15);

	const char* Options = "0123456789abcdef";
	constexpr bool Dash[] = {false, false, false, false, true, false, true, false, true, false, true, false, false, false, false, false};

	std::string Result;
	for (int i = 0; i < 16; i++)
	{
		if (Dash[i]) Result += "-";
		Result += Options[Distribution(RNG)];
		Result += Options[Distribution(RNG)];
	}
	return Result.c_str();
}
