// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MazeChase : ModuleRules
{
	public MazeChase(TargetInfo Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
        PublicDependencyModuleNames.AddRange(new string[] { "UMG", "Slate", "SlateCore" });
        PrivateDependencyModuleNames.AddRange(new string[] { "AIModule", "GameplayTasks" });

        PrivateDependencyModuleNames.AddRange(new string[] { "FMODStudio" });
        PublicIncludePaths.AddRange(new string[] { "FMODStudio/Source/FMODStudio/Public" });
        PublicIncludePaths.AddRange(new string[] { "Plugins/FMODStudio/Source/FMODStudio/Public/FMOD/" });
    }
}
