// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Witcher3 : ModuleRules
{
	public Witcher3(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput","GameplayAbilities","GameplayTags", "GameplayTasks", "MotionWarping","AnimationCore","UMG","Slate","SlateCore","CommonUI", "ProceduralDismembermentSystem","AIModule", "NavigationSystem", "Niagara"});
        if (Target.bBuildEditor)
        {
            PrivateDependencyModuleNames.AddRange(new string[] { "AnimGraph" });
        }
    }
}
