using System;
using System.Collections.Generic;
using System.Runtime.Intrinsics.X86;
using System.Xml;
using UnrealBuildTool;

public class InkpotDemoEditorTarget : TargetRules
{
	public InkpotDemoEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.Latest;
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
        NativePointerMemberBehaviorOverride = PointerMemberBehavior.Disallow;
        ExtraModuleNames.Add("InkpotDemo");
		ExtraModuleNames.Add("InkpotDemoEditor");
	}
}
