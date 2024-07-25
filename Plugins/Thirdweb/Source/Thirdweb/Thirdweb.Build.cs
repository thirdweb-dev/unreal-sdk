using UnrealBuildTool;
using System.IO;

public class Thirdweb : ModuleRules
{
    public Thirdweb(ReadOnlyTargetRules Target)
        : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        // Path to the library file
        string lib = Path.Combine(ModuleDirectory, "..", "..", "lib", "thirdweb.lib");

        // Path to the header files
        string includes = Path.Combine(ModuleDirectory, "Public");

        // Add the library file to the public additional libraries
        PublicAdditionalLibraries.Add(lib);

        // Explicitly add the required system libraries
        PublicSystemLibraries.AddRange(
            new string[]
            {
                "Advapi32.lib",
                "Ntdll.lib",
                "bcrypt.lib",
                "userenv.lib",
                "Crypt32.lib",
                "Secur32.lib",
                "Ncrypt.lib"
            }
        );

        // Add the include path to the public include paths
        PublicIncludePaths.Add(includes);

        // Add the source code directory to public include paths
        PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Public"));
        PrivateIncludePaths.Add(Path.Combine(ModuleDirectory, "Private"));

        // Add dependencies
        PublicDependencyModuleNames.AddRange(
            new string[] { "Core", "CoreUObject", "Engine", "HTTP", "HTTPServer", "Networking" }
        );
    }
}
