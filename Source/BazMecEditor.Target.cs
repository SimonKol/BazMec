// (c) Simon Kolciter

using UnrealBuildTool;
using System.Collections.Generic;

public class BazMecEditorTarget : TargetRules
{
	public BazMecEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.Latest;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		ExtraModuleNames.AddRange(new string[]{ "BazMec", "BazMecEd"});
	}
}
