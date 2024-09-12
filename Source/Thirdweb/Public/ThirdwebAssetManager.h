// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Materials/Material.h"

#include "UObject/SoftObjectPtr.h"

#include "ThirdwebAssetManager.generated.h"

class UMaterial;
/**
 * @class UThirdwebAssetManager
 * @brief Manages and handles loading, unloading, and accessing game assets.
 *
 * The UThirdwebAssetManager class is responsible for the efficient management
 * of game assets such as textures, models, sound files, and other resources.
 * It provides functions to load and unload assets, handles asset reference
 * counting, and ensures that assets are loaded only once. This helps in
 * optimizing game performance by minimizing redundant loading and memory usage.
 */
UCLASS()
class THIRDWEB_API UThirdwebAssetManager : public UObject
{
	GENERATED_BODY()
public:
	UThirdwebAssetManager();
	
	void LoadDefaultMaterials();

	UMaterial* GetDefaultMaterial(); 
	UMaterial* GetDefaultTranslucentMaterial(); 

protected:
	UPROPERTY()
	TSoftObjectPtr<UMaterial> DefaultMaterial;
	TSoftObjectPtr<UMaterial> DefaultTranslucentMaterial;
};
