// Copyright (c) 2024 Thirdweb. All Rights Reserved.

using UnrealBuildTool;

public class ThirdwebEditor : ModuleRules
{
	public ThirdwebEditor(ReadOnlyTargetRules target) : base(target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
#if UE_5_3_OR_LATER
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
#endif

		PublicDependencyModuleNames.AddRange(new[]
		{
			"Core",
			"Thirdweb",
			"DeveloperSettings",
			"ImageWriteQueue",
			"UMG",
			"ApplicationCore"
		});


		PrivateDependencyModuleNames.AddRange(new[]
		{
			"Projects",
			"InputCore",
			"EditorFramework",
			"UnrealEd",
			"ToolMenus",
			"CoreUObject",
			"Engine",
			"Slate",
			"SlateCore",
			"RenderCore"
		});
	}
}