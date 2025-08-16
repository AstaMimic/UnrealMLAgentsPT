using UnrealBuildTool;

public class SimCadenceEditor : ModuleRules
{
    public SimCadenceEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PrivateDependencyModuleNames.AddRange(new string[] {
            "Core", "CoreUObject", "Engine",
            "UnrealEd",
            "SimCadenceRuntime"
        });

        if (!Target.bBuildEditor)
        {
            throw new BuildException("SimCadenceEditor must be built with the editor.");
        }
    }
}
