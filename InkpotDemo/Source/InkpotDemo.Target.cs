using UnrealBuildTool;
using System.Collections.Generic;

public class InkpotDemoTarget : TargetRules
{
	public InkpotDemoTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.V5;
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
        ExtraModuleNames.Add("InkpotDemo");
	}
}
