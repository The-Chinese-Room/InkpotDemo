using UnrealBuildTool;
using System.Collections.Generic;

public class InkpotDemoEditorTarget : TargetRules
{
	public InkpotDemoEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V3;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_3;
		ExtraModuleNames.Add("InkpotDemo");
		ExtraModuleNames.Add("InkpotDemoEditor");
	}
}
