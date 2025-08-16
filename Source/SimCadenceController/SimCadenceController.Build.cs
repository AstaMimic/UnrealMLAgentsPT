using UnrealBuildTool;

public class SimCadenceController : ModuleRules
{
	public SimCadenceController(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "DeveloperSettings" });

		PrivateDependencyModuleNames.AddRange(new string[] { "UnrealEd", "Slate", "SlateCore", "Projects" });

		if (!Target.bBuildEditor)
		{
			throw new BuildException("SimCadenceController must be built with the editor.");
		}
	}
}
