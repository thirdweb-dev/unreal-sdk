using UnrealBuildTool;

public class thirdweb_unreal : ModuleRules
{
    public thirdweb_unreal(ReadOnlyTargetRules Target)
        : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PublicDependencyModuleNames.AddRange(
            new string[] { "Core", "CoreUObject", "Engine", "InputCore", "Thirdweb" }
        );
        PrivateDependencyModuleNames.AddRange(new string[] { });
    }
}
