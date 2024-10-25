// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "ThirdwebModule.h"

#include "IWebBrowserSingleton.h"
#include "ThirdwebAssetManager.h"
#include "ThirdwebLog.h"
#include "WebBrowserModule.h"

#include "Materials/Material.h"

class FThirdwebModule : public IThirdwebModule
{
public:
	virtual void StartupModule() override
	{
		if (ThirdwebAssetManager == nullptr)
		{
			ThirdwebAssetManager = NewObject<UThirdwebAssetManager>((UObject*)GetTransientPackage(), NAME_None, RF_Transient | RF_Public);
			ThirdwebAssetManager->LoadDefaultMaterials();

			IWebBrowserModule::Get(); // force the module to load
			if (IWebBrowserModule::IsAvailable() && IWebBrowserModule::Get().IsWebModuleAvailable())
			{
				IWebBrowserModule& WebBrowserModule = IWebBrowserModule::Get();
				FWebBrowserInitSettings WebBrowserInitSettings;
				// Needed to make google oauth happy on mobile
				WebBrowserInitSettings.ProductVersion = TEXT("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/128.0.0.0 Safari/537.36");
				if (!WebBrowserModule.CustomInitialize(WebBrowserInitSettings))
				{
					TW_LOG(Error, TEXT("ThirdwebModule::Could not initialize WebBrowserModule with custom User-Agent! Singleton already exists"))
				}
				IWebBrowserSingleton* WebBrowserSingleton = WebBrowserModule.GetSingleton();
				if (WebBrowserSingleton)
				{
					WebBrowserSingleton->SetDefaultMaterial(ThirdwebAssetManager->GetDefaultMaterial());
					WebBrowserSingleton->SetDefaultTranslucentMaterial(ThirdwebAssetManager->GetDefaultTranslucentMaterial());
				}
			}
		}
	}

	virtual void ShutdownModule() override
	{
	}

private:
	UThirdwebAssetManager* ThirdwebAssetManager = nullptr;
};

IMPLEMENT_MODULE(FThirdwebModule, Thirdweb);
