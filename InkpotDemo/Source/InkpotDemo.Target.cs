using UnrealBuildTool;
using System.Collections.Generic;

public class InkpotDemoTarget : TargetRules
{
	public InkpotDemoTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_2;
		ExtraModuleNames.Add("InkpotDemo");
	}
}
