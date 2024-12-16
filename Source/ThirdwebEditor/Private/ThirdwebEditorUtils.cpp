// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#include "ThirdwebEditorUtils.h"

#include "ISettingsModule.h"
#include "Modules/ModuleManager.h"

namespace ThirdwebEditorUtils
{
	template <typename T>
	void OpenDeveloperSettings()
	{
		const T* Settings = GetDefault<T>();
		FModuleManager::LoadModuleChecked<ISettingsModule>("Settings").ShowViewer(Settings->GetContainerName(), Settings->GetCategoryName(), Settings->GetSectionName());
	}
}
