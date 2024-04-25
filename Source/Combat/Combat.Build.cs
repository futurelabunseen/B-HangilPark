// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Combat : ModuleRules
{
	public Combat(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PrivateIncludePaths.AddRange(new string[] { 
			"Combat" 
		});

        PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore",
            "EnhancedInput",
            "UMG",
	        "GameplayAbilities",
            "GameplayTasks",
            "GameplayTags",
            "Niagara",
            "PhysicsCore"
        });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
