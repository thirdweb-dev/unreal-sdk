// Copyright (c) 2024 Thirdweb. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class Thirdweb : ModuleRules
{
	public Thirdweb(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		
		// Add the library file to the public additional libraries
		PublicAdditionalLibraries.Add(Path.Combine(PluginDirectory, "lib", "thirdweb.lib"));

		// Explicitly add the required system libraries
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


		// Add dependencies
		PublicDependencyModuleNames.AddRange(new[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"HTTP",
			"HTTPServer",
			"Networking"
		});
	}
}