using UnrealBuildTool;
using System.Collections.Generic;

public class InkpotDemoEditorTarget : TargetRules
{
	public InkpotDemoEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_2;
		ExtraModuleNames.Add("InkpotDemo");
		ExtraModuleNames.Add("InkpotDemoEditor");
	}
}
