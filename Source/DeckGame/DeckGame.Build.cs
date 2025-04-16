// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class DeckGame : ModuleRules
{
	public DeckGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"GameplayAbilities",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule"
		});

		PrivateDependencyModuleNames.AddRange(new string[] {
			"AIModule",
			"GameplayAbilities",
            "GameplayMessageRuntime",
            "GameplayStateTreeModule",
            "GameplayTags",
			"GameplayTasks",
			"NavigationSystem",
			"StateTreeModule",
			"UMG"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
