using UnrealBuildTool;
using System.Collections.Generic;

public class InkpotDemoEditorTarget : TargetRules
{
	public InkpotDemoEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.V5;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_4;
        ExtraModuleNames.Add("InkpotDemo");
		ExtraModuleNames.Add("InkpotDemoEditor");
	}
}
