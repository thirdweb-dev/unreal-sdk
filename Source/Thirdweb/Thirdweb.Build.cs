// Copyright (c) 2024 Thirdweb. All Rights Reserved.

using UnrealBuildTool;
using System.IO;
using Microsoft.Extensions.Logging;

public class Thirdweb : ModuleRules
{
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

		var libDirName = target.Platform.IsInGroup(UnrealPlatformGroup.IOS) ? "IOS" : target.Platform.ToString();
		var libDir = Path.Combine(Path.Combine(ModuleDirectory, "..", "ThirdParty"), libDirName);
		PublicSystemLibraryPaths.Add(libDir);
		
		var libName = target.Platform.Equals(UnrealTargetPlatform.Win64) ? "thirdweb.lib" : "libthirdweb.a";
		PublicAdditionalLibraries.Add(Path.Combine(libDir, libName));
		
		if (target.Platform.Equals(UnrealTargetPlatform.Win64))
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

		if (target.Platform.Equals(UnrealTargetPlatform.Mac) || target.Platform.Equals(UnrealTargetPlatform.IOS))
		{
			PublicFrameworks.AddRange(new []{"SystemConfiguration", "Foundation", "AuthenticationServices"});
			PrivateIncludePaths.AddRange(new [] { Path.Combine(ModuleDirectory, "Private", target.Platform.ToString()) });
			PrivateDependencyModuleNames.Add("CEF3Utils");
		}

		if (target.Platform.Equals(UnrealTargetPlatform.IOS) || target.Platform.Equals(UnrealTargetPlatform.Android))
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