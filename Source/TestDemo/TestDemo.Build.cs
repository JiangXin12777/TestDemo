// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TestDemo : ModuleRules
{
	public TestDemo(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"HeadMountedDisplay", 
			"EnhancedInput",
			"GameplayTags",
			"GameplayTasks", 
			"GameplayAbilities",
			"FabulousAbilitySystem",
			"ModularGameplayActors",
			"TargetSystem",
			"ALS",
			"DeveloperSettings",
			"ALSCamera",
			"NinjaInput",
			"PhysicsCore",
			"GameplayMessageRuntime",
			"ModularGameplay",
			"NetCore",
			"IrisCore",
			"UMG",
			"Niagara",
			"SlateCore"
		});
	}
}
