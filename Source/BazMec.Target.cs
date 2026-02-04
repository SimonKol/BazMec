// (c) Simon Kolciter

using UnrealBuildTool;
using System.Collections.Generic;

public class BazMecTarget : TargetRules
{
	public BazMecTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.Latest;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		ExtraModuleNames.Add("BazMec");
	}
}
