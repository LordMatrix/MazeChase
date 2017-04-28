// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MazeChase : ModuleRules
{
	public MazeChase(TargetInfo Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
        PrivateDependencyModuleNames.AddRange(new string[] { "AIModule", "GameplayTasks" });

        PrivateDependencyModuleNames.AddRange(new string[] { "FMODStudio" });
        PublicIncludePaths.AddRange(new string[] { "FMODStudio/Source/FMODStudio/Public" });
    }
}
