using UnrealBuildTool;

public class SimCadenceController : ModuleRules
{
	public SimCadenceController(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[] { "Core", "CoreUObject", "Engine", "Projects", "DeveloperSettings" });

		PrivateDependencyModuleNames.AddRange(
			new string[] { "Slate", "SlateCore", "InputCore", "RenderCore", "RHI", "PhysicsCore" });

		if (Target.bBuildEditor)
		{
			PrivateDependencyModuleNames.AddRange(new string[] { "UnrealEd", "Settings", "EditorFramework" });
			PublicDefinitions.Add("WITH_SIMCADENCE_TRAINING_ENGINE=1");
		}
		else
		{
			PublicDefinitions.Add("WITH_SIMCADENCE_TRAINING_ENGINE=0");
		}
	}
}
