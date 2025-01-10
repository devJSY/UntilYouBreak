// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UntilYouBreak : ModuleRules
{
    public UntilYouBreak(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(new string[] { "UntilYouBreak" });

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "NavigationSystem", "AIModule", "Niagara", "UMG" });
    }
}
