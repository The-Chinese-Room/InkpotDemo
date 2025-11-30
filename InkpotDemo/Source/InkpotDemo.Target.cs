using UnrealBuildTool;
using System.Collections.Generic;

public class InkpotDemoTarget : TargetRules
{
	public InkpotDemoTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.Latest;
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
        ExtraModuleNames.Add("InkpotDemo");
	}
}
