#include "ThirdwebInternal.h"

#include <random>

#include "HttpModule.h"
#include "ThirdwebRuntimeSettings.h"
#include "ThirdwebUtils.h"

#include "Dom/JsonObject.h"

#include "Interfaces/IPluginManager.h"

#include "Kismet/GameplayStatics.h"

#include "Policies/CondensedJsonPrintPolicy.h"

#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"

FString FThirdwebAnalytics::JsonObjectToString(const TSharedPtr<FJsonObject>& JsonObject)
{
	FString Out;
	const TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&Out);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
	return Out;
}

FString FThirdwebAnalytics::GetPluginVersion()
{
	if (const TSharedPtr<IPlugin> Plugin = IPluginManager::Get().FindPlugin(TEXT("Thirdweb")); Plugin.IsValid())
	{
		return Plugin->GetDescriptor().VersionName;
	}
	return "0.0.0";
}

void FThirdwebAnalytics::SendConnectEvent(const FString& Wallet, const FString& Type)
{
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
	JsonObject->SetStringField(TEXT("walletAddress"), Wallet);
	JsonObject->SetStringField(TEXT("walletType"), Type);
	Request->SetContentAsString(JsonObjectToString(JsonObject));
	Request->ProcessRequest();
}

// https://stackoverflow.com/a/58467162/12204515
FString FThirdwebAnalytics::GenerateUUID()
{
	static std::random_device Device;
	static std::mt19937 RNG(Device());

	std::uniform_int_distribution<int> Distribution(0, 15);

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
