// Copyright (c) 2024 Thirdweb. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class Thirdweb : ModuleRules
{
	private bool IsWin64 => Target.Platform.Equals(UnrealTargetPlatform.Win64);

	private bool IsIOSIsh =>
		Target.Platform.IsInGroup(UnrealPlatformGroup.IOS)
#if UE_5_4_OR_LATER
		|| Target.Platform.Equals(UnrealTargetPlatform.VisionOS)
#endif
	;

	private bool IsApple => Target.Platform.IsInGroup(UnrealPlatformGroup.Apple);
	
	private bool IsAndroid => Target.Platform.Equals(UnrealTargetPlatform.Android);

	private bool IsMobile => IsIOSIsh || IsAndroid;

	private string LibExt =>
		IsWin64 ? ".lib" : Target.Architectures.Contains(UnrealArch.IOSSimulator) ? ".sim.a" : ".a";

	private string LibDir => Path.Combine(Path.Combine(ModuleDirectory, "..", "ThirdParty"),
		IsIOSIsh ? "IOS" : Target.Platform.ToString());

	public Thirdweb(ReadOnlyTargetRules target) : base(target)
	{
		PrivateDependencyModuleNames.AddRange(new [] { "Boost" });
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

#if UE_5_3_OR_LATER
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
#endif

#if UE_5_0_OR_LATER
		PublicDefinitions.Add("WITH_CEF=1");
		PrivateDependencyModuleNames.Add("WebBrowser");
		
		// Copied from WebBrowserWidget
		if (target.bBuildEditor || IsAndroid || IsIOSIsh)
		{
			// WebBrowserTexture required for cooking Android
			PrivateIncludePathModuleNames.AddRange(new [] { "WebBrowserTexture" });
			PrivateDependencyModuleNames.AddRange(new [] {  "WebBrowserTexture" });

			if (IsAndroid)
			{
				var PluginPath = Utils.MakePathRelativeTo(ModuleDirectory, target.RelativeEnginePath);
				AdditionalPropertiesForReceipt.Add("AndroidPlugin", Path.Combine(PluginPath, "Thirdweb_UPL.xml"));
			}
		}
#else
		PublicDefinitions.Add("WITH_CEF=0");
#endif

		PublicSystemLibraryPaths.Add(LibDir);
		PublicAdditionalLibraries.Add(Path.Combine(LibDir, "libthirdweb" + LibExt));

		if (IsWin64)
		{
			PublicSystemLibraries.AddRange(new[]
			{
				"Advapi32.lib",
				"Ntdll.lib",
				"bcrypt.lib",
				"userenv.lib",
				"Crypt32.lib",
				"Secur32.lib",
				"Ncrypt.lib"
			});
		}

		if (IsApple)
		{
			PublicFrameworks.AddRange(new[] { "SystemConfiguration", "Foundation", "AuthenticationServices" });
			PrivateDependencyModuleNames.Add("CEF3Utils");
		}

		if (IsMobile)
		{
			PrivateDependencyModuleNames.Add("Launch");
		}
		
		PublicDependencyModuleNames.AddRange(new[]
		{
			// Standard deps
			"Core",
			"CoreUObject",
			"Engine",
			// plugin settings deps
			"DeveloperSettings",
			// Thirdweb Engine call deps
			"HTTP",
			"HTTPServer",
			"Networking",
			"Json",
			// OAuth Widget UI deps
			"UMG",
			"Slate",
			"SlateCore"
		});
		
		// Copied from WebBrowserWidget
		if (target.bBuildEditor)
		{
			// @TODO: UnrealEd Needed for the triangulation code used for sprites (but only in editor mode)
			// @TOOD: Try to move the code dependent on the triangulation code to the editor-only module
			PrivateIncludePathModuleNames.AddRange(new [] { "UnrealEd" });
			PrivateDependencyModuleNames.AddRange(new [] { "EditorFramework", "UnrealEd" });
		}
	}
}