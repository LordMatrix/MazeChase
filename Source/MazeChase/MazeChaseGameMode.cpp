// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "MazeChase.h"
#include "MazeChaseGameMode.h"
#include "MazeChaseCharacter.h"

AMazeChaseGameMode::AMazeChaseGameMode() {
	// set default pawn class to our Blueprinted character
	/*
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/ThirdPersonCharacter"));

	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Characters/Hero/BP_Hero"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	*/
}

void AMazeChaseGameMode::Tick(float DeltaSeconds) {

}
