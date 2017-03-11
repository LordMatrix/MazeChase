// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Components/ChildActorComponent.h"
#include "Cell.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include "Labyrinth.generated.h"

UCLASS()
class MAZECHASE_API ALabyrinth : public AActor
{
	GENERATED_BODY()
	
public:	
	ALabyrinth();

	virtual void BeginPlay() override;
	
	virtual void Tick( float DeltaSeconds ) override;

	void generate();
	
	
	Cell getCell(int x, int y);

	UFUNCTION(BlueprintCallable, category = "Maze")
	int getWallsAt(int x, int y);

	// Wall object
	UPROPERTY(EditAnywhere, Category = Config)
	TSubclassOf<AActor> wall_class_;
	UChildActorComponent* wallsubs[30][30];
	// Create constants (ROWS, COLS) to store the size of the maze.
	static const int ROWS = 30;
	static const int COLS = 30;

	static const int WALL_SIZE = 100;

private:
	// Create a 2-D array ([ROWS][COLS]) of Cell objects.
	Cell maze_[ROWS][COLS];

	//Starting position
	int startX, startY;

	//Exit position
	int exitX, exitY;
};
