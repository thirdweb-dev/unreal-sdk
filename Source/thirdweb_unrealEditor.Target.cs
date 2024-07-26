using UnrealBuildTool;
using System.Collections.Generic;

public class thirdweb_unrealEditorTarget : TargetRules
{
    public thirdweb_unrealEditorTarget(TargetInfo Target)
        : base(Target)
    {
        Type = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.V5;
        ExtraModuleNames.AddRange(new string[] { "thirdweb_unreal" });
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
    }
}
