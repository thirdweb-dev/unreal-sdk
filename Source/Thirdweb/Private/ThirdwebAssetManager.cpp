// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "ThirdwebAssetManager.h"

#if WITH_EDITOR || PLATFORM_ANDROID || PLATFORM_IOS
#include "WebBrowserTexture.h"

#include "Materials/Material.h"
#endif

UThirdwebAssetManager::UThirdwebAssetManager()
{
	DefaultMaterial = FString(TEXT("/Thirdweb/Assets/WebTexture_M.WebTexture_M"));
	DefaultTranslucentMaterial = FString(TEXT("/Thirdweb/Assets/WebTexture_TM.WebTexture_TM"));
	
#if WITH_EDITOR || PLATFORM_ANDROID || PLATFORM_IOS
	// Add a hard reference to UWebBrowserTexture, without this the WebBrowserTexture DLL never gets loaded on Windows.
	UWebBrowserTexture::StaticClass();
#endif
}

void UThirdwebAssetManager::LoadDefaultMaterials()
{
	DefaultMaterial.LoadSynchronous();
	DefaultTranslucentMaterial.LoadSynchronous();
}

UMaterial* UThirdwebAssetManager::GetDefaultMaterial()
{
	return DefaultMaterial.Get();
}

UMaterial* UThirdwebAssetManager::GetDefaultTranslucentMaterial()
{
	return DefaultTranslucentMaterial.Get();
}
