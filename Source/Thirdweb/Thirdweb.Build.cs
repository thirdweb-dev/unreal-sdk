// Copyright (c) 2024 Thirdweb. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class Thirdweb : ModuleRules
{
	private bool IsWin64 => Target.Platform.Equals(UnrealTargetPlatform.Win64);

	private bool IsIOSIsh => Target.Platform.IsInGroup(UnrealPlatformGroup.IOS) ||
	                         Target.Platform.Equals(UnrealTargetPlatform.VisionOS);

	private bool IsApple => Target.Platform.IsInGroup(UnrealPlatformGroup.Apple);

	private bool IsMobile => IsIOSIsh || Target.Platform.Equals(UnrealTargetPlatform.Android);

	private string LibExt => IsWin64 ? ".lib" : Target.Architecture == UnrealArch.IOSSimulator ? ".sim.a" : ".a";

	private string LibDir => Path.Combine(Path.Combine(ModuleDirectory, "..", "ThirdParty"),
		IsIOSIsh ? "IOS" : Target.Platform.ToString());


	public Thirdweb(ReadOnlyTargetRules target) : base(target)
	{
		PrivateDependencyModuleNames.AddRange(new string[] { "Boost" });
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

#if UE_5_3_OR_LATER
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
#endif

#if UE_5_0_OR_LATER
		PublicDefinitions.Add("WITH_CEF=1");
		PublicDependencyModuleNames.Add("WebBrowserWidget");
		PrivateDependencyModuleNames.Add("WebBrowser");
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
	}
}