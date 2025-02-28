// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class DeckUI : ModuleRules
{
	public DeckUI(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"GameplayTags",
			"AIModule",
			"CommonUI",
			"CommonInput",
			"UMG",
			"ModelViewViewModel"
        });

		PrivateDependencyModuleNames.AddRange(new string[] {
            "DeckGame",
			"GameplayAbilities"
        });
	}
}
