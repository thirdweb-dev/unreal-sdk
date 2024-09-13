// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "ThirdwebModule.h"

#include "IWebBrowserSingleton.h"
#include "ThirdwebAssetManager.h"
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
				IWebBrowserSingleton* WebBrowserSingleton = IWebBrowserModule::Get().GetSingleton();
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

