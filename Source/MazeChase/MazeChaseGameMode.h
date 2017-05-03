// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/GameMode.h"
#include "MazeChaseGameMode.generated.h"

UCLASS(minimalapi)
class AMazeChaseGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AMazeChaseGameMode();
	void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "funcs")
		void fadeToMenu();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
		bool game_won_;
};



