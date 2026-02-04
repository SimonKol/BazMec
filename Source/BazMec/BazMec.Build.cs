// (c) Simon Kolciter

using UnrealBuildTool;

public class BazMec : ModuleRules
{
	public BazMec(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });

		PrivateDependencyModuleNames.AddRange(new string[] { "UMG", "Slate", "SlateCore", "DeveloperSettings", "GameplayTags", "RenderCore" });

		PrivateIncludePaths.Add("BazMec");
	}
}
