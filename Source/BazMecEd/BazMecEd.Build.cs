// (c) Simon Kolciter

using UnrealBuildTool;

public class BazMecEd : ModuleRules
{
	public BazMecEd(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine" });

		PrivateDependencyModuleNames.AddRange(new string[] { "UMG", "Slate", "SlateCore" });
	}
}
