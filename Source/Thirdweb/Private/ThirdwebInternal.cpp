#include "ThirdwebInternal.h"

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
	if (!Settings->bSendAnalytics || (Settings->BundleID.IsEmpty() && Settings->ClientID.IsEmpty() && Settings->SecretKey.IsEmpty()))
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
	if (!Settings->SecretKey.IsEmpty())
	{
		Request->SetHeader("x-client-id", ThirdwebUtils::GetClientIdFromSecretKey(Settings->SecretKey));
	} else
	{
		Request->SetHeader("x-client-id", Settings->ClientID);
		Request->SetHeader("x-bundle-id", Settings->BundleID);
	}
	Request->SetTimeout(5.0f);

	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	JsonObject->SetStringField(TEXT("source"), TEXT("connectWallet"));
	JsonObject->SetStringField(TEXT("action"), TEXT("connect"));
	JsonObject->SetStringField(TEXT("walletAddress"), Wallet);
	JsonObject->SetStringField(TEXT("walletType"), Type);
	Request->SetContentAsString(JsonObjectToString(JsonObject));
	Request->ProcessRequest();
}
