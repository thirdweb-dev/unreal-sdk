// Copyright (c) 2024 Thirdweb. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class Thirdweb : ModuleRules
{
	public Thirdweb(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		var baseLibDir = Path.Combine(ModuleDirectory, "..", "ThirdParty");
		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			var libDir = Path.Combine(baseLibDir, "Win64");
			PublicSystemLibraryPaths.Add(libDir);
			PublicAdditionalLibraries.Add(Path.Combine(libDir, "thirdweb.lib"));
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
		} else if (Target.Platform == UnrealTargetPlatform.Linux)
		{
			var libDir = Path.Combine(baseLibDir, "Linux");
			PublicSystemLibraryPaths.Add(libDir);
			PublicAdditionalLibraries.Add(Path.Combine(libDir, "libthirdweb.a"));
		}  else if (Target.Platform == UnrealTargetPlatform.LinuxArm64)
		{
			var libDir = Path.Combine(baseLibDir, "LinuxARM64");
			PublicSystemLibraryPaths.Add(libDir);
			PublicAdditionalLibraries.Add(Path.Combine(libDir, "libthirdweb.a"));
		} else if (Target.Platform == UnrealTargetPlatform.Mac)
		{
			var libDir = Path.Combine(baseLibDir, "Mac");
			PublicSystemLibraryPaths.Add(libDir);
			PublicAdditionalLibraries.Add(Path.Combine(libDir, "libthirdweb.a"));
		} else if (Target.Platform == UnrealTargetPlatform.IOS)
		{
			var libDir = Path.Combine(baseLibDir, "IOS");
			PublicSystemLibraryPaths.Add(libDir);
			PublicAdditionalLibraries.Add(Path.Combine(libDir, "libthirdweb.a"));
		}

		PublicDependencyModuleNames.AddRange(new[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"HTTP",
			"HTTPServer",
			"Networking",
			"DeveloperSettings",
			"Json"
		});
	}
}