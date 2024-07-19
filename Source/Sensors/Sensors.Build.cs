using UnrealBuildTool;
using System;
using System.IO;
using System.Collections.Generic;
using System.Reflection;

public class Sensors: ModuleRules
{
    public Sensors(ReadOnlyTargetRules Target) : base(Target)
    {
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(
            new string[] {
				Path.Combine(ModuleDirectory, "Public"),
			}
		);

		PrivateIncludePathModuleNames.AddRange(new string[] { "DRL", "Policies", "Communicator" });

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Actuators",
			}
		);

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine"
			}
		);
	}
}
