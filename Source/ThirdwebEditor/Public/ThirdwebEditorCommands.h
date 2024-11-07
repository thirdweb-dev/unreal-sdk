// Copyright (c) 2024 Thirdweb. All Rights Reserved.

#pragma once

#include "ThirdwebEditorStyle.h"

#include "Framework/Commands/Commands.h"

class FThirdwebEditorCommands : public TCommands<FThirdwebEditorCommands>
{
public:
	FThirdwebEditorCommands();

	virtual void RegisterCommands() override;

	static TSharedPtr<FUICommandInfo> GetOpenSettingsCommand() { return Get().OpenSettings; }
	static TSharedPtr<FUICommandInfo> GetTakeScreenshotCommand() { return Get().TakeScreenshot; }
	// ReSharper disable once CppConstValueFunctionReturnType
	static const TArray<TSharedPtr<FUICommandInfo>> GetCommands() { return {Get().OpenSettings, Get().TakeScreenshot}; }

	template <typename CallableT>
	static void ForEach(CallableT Callable)
	{
		// ReSharper disable once CppTooWideScopeInitStatement
		const TArray<TSharedPtr<FUICommandInfo>> Commands = GetCommands();
		for (const TSharedPtr<FUICommandInfo>& Value : Commands)
		{
			Invoke(Callable, Value);
		}
	}

protected:
	TSharedPtr<FUICommandInfo> OpenSettings;
	TSharedPtr<FUICommandInfo> TakeScreenshot;
};
