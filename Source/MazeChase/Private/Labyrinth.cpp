// Fill out your copyright notice in the Description page of Project Settings.

#include "MazeChase.h"
#include "Labyrinth.h"

// Sets default values
ALabyrinth::ALabyrinth(){
	PrimaryActorTick.bCanEverTick = true;

}


void ALabyrinth::OnConstruction(const FTransform& Transform) {
	// Generate maze model
	generateModel();
	fill();

	carveWalls();
	createExit();
	createPlayerStart();
}


void ALabyrinth::BeginPlay() {
	Super::BeginPlay();

}


void ALabyrinth::Tick( float DeltaTime ){
	Super::Tick( DeltaTime );
}


void ALabyrinth::rebuild() {
	//Tear down all walls
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLS; j++) {
			for (int k = 0; k < 4; k++) {
				if (IsValid(wallsubs[i][j][k]))
					wallsubs[i][j][k]->DestroyComponent();
			}
		}
	}

	generateModel();
	fill();
	carveWalls();
	createExit();
}


void ALabyrinth::generateModel() {
	using std::cout;
	using std::vector;

	// Create an enum named DIR to keep track of the four directions (NORTH, EAST, SOUTH, WEST)
	enum DIR { NORTH, SOUTH, EAST, WEST };

	// Randomize the random number function.
	srand(time(NULL));

	// For each Cell in the maze_:
	for (int row = 0; row < ROWS; row++)
		for (int col = 0; col < COLS; col++) {
			// set visited to false
			maze_[row][col].setVisited(false);
			// set its position to its row and column in the maze_
			maze_[row][col].setPosition(row, col);
			// set the Cell's walls to Cell::WALL_ALL
			maze_[row][col].setWalls(Cell::WALL_ALL);
		}

	//Create curX and curY variables and set them to a random position in the maze_.
	int curX = rand() % ROWS;
	int curY = rand() % COLS;

	//Set starting position
	startX = curX;
	startY = curY;

	// Create a vector of Cell objects named trail which will be used as a stack.
	vector<Cell> trail;

	// Create a vector of DIR values named live.
	vector<DIR> live;

	// Grab the Cell at the curX, curY position and push it on the trail stack.
	trail.push_back(maze_[curX][curY]);

	// While the trail stack is not empty do the following:
	while (trail.empty() == false) { // stay in here till display
									 // Empty the live vector.
		live.clear();
		// Check the neighbors of the current cell to the north, east, south, and west.
		// If any of the neighbors have all four walls, add the direction to that 
		// neighbor to the live vector.
		if (curY)
			if (maze_[curX][curY - 1].getWalls() == Cell::WALL_ALL) // West has all walls
				live.push_back(WEST);
		if (curY<COLS - 1)
			if (maze_[curX][curY + 1].getWalls() == Cell::WALL_ALL) // east has all walls
				live.push_back(EAST);
		if (curX)
			if (maze_[curX - 1][curY].getWalls() == Cell::WALL_ALL) // North has all walls
				live.push_back(NORTH);
		if (curX<ROWS - 1)
			if (maze_[curX + 1][curY].getWalls() == Cell::WALL_ALL) // South has all walls
				live.push_back(SOUTH);
		// If the live vector is not empty:
		if (live.empty() == false) {
			// Choose one of the directions in the live vector at random
			// ran_dir=rand() % live.size();
			// cout << "Random dir " << ran_dir << " out of " << live.size() << "\n";
			// Remove the walls between the current cell and the neighbor in that direction
			// and Change curX and curY to refer to the neighbor
			maze_[curX][curY].setVisited(true);
			int direction = live[rand() % live.size()];

			switch (direction) {
			case 0:
				maze_[curX][curY].removeWall(Cell::WALL_NORTH);
				maze_[--curX][curY].removeWall(Cell::WALL_SOUTH);
				break;
			case 1:
				maze_[curX][curY].removeWall(Cell::WALL_SOUTH);
				maze_[++curX][curY].removeWall(Cell::WALL_NORTH);
				break;
			case 2:
				maze_[curX][curY].removeWall(Cell::WALL_EAST);
				maze_[curX][++curY].removeWall(Cell::WALL_WEST);
				break;
			case 3:
				maze_[curX][curY].removeWall(Cell::WALL_WEST);
				maze_[curX][--curY].removeWall(Cell::WALL_EAST);
				break;
			}

			//Mark as exit if this is a map boundary
			if (curX == 0 || curX == ROWS - 1 || curY == 0 || curY == COLS - 1) {
				exitX = curX;
				exitY = curY;
			}

			// Push the new current cell onto the trail stack
			/*cout << "maze_ " << curX << ", " << curY << "\n";
			cin.ignore(); */
			trail.push_back(maze_[curX][curY]);
		} //If the live vector was empty:
		else {
			// Pop the top item from the trail stack
			trail.pop_back();

			// If the trail stack is not empty, set curX and curY to refer to the 
			// position of the top item in the trail stack.
			if (trail.empty() == false) {
				//curX = trail[0].getRow();
				//curY = trail[0].getColumn();
				curX = trail[trail.size() - 1].getRow();
				curY = trail[trail.size() - 1].getColumn();
			}
		}
	}
}


void ALabyrinth::fill() {
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLS; j++) {
			//WE CREATE 4 WALLS PER CELL.
			for (int k = 0; k < 4; k++) {
				FString name = "ChildWall_" + FString::FromInt(i) + "_" + FString::FromInt(j) + FString::FromInt(k);
				//wallsubs[i][j][k] = CreateDefaultSubobject<UStaticMeshComponent>(FName(*name));
				wallsubs[i][j][k] = ConstructObject<UStaticMeshComponent>(UStaticMeshComponent::StaticClass(), this, FName(*name));
			}
		}
	}
}


void ALabyrinth::carveWalls() {
	/// Create labyrinth of static meshes from the generated model
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLS; j++) {
			int walls = getWallsAt(i, j);


			for (int k = 0; k < 4; k++) {
				if (walls & Cell::WALL_ALL) {
					UStaticMeshComponent* wallsub = wallsubs[i][j][k];

					wallsub->SetStaticMesh(wall_class_);
					wallsub->SetRelativeScale3D(FVector(5.0f, 0.2f, 5.0f));

					float x = i * WALL_SIZE;
					float y = j * WALL_SIZE;

					switch (k) {
					case 0:
						if (walls & Cell::WALL_NORTH) {
							wallsub->SetRelativeRotation(FQuat(FVector(0.0f, 0.0f, 1.0f), 1.57f));
							wallsub->SetRelativeLocation(FVector(x - (WALL_SIZE / 2.0f), y, 0.0f));
						}
						else
							wallsubs[i][j][k]->DestroyComponent();
						break;
					case 1:
						if (walls & Cell::WALL_EAST) {
							wallsub->SetRelativeLocation(FVector(x, y + (WALL_SIZE / 2.0f), 0.0f));
						}
						else
							wallsubs[i][j][k]->DestroyComponent();
						break;
					case 2:
						if (walls & Cell::WALL_SOUTH) {
							wallsub->SetRelativeRotation(FQuat(FVector(0.0f, 0.0f, 1.0f), 1.57f));
							wallsub->SetRelativeLocation(FVector(x + (WALL_SIZE / 2.0f), y, 0.0f));
						}
						else
							wallsubs[i][j][k]->DestroyComponent();
						break;
					case 3:
						if (walls & Cell::WALL_WEST) {
							wallsub->SetRelativeLocation(FVector(x, y - (WALL_SIZE / 2.0f), 0.0f));
						}
						else
							wallsubs[i][j][k]->DestroyComponent();
						break;
					}
				}
				else {
					//If there are no walls, destroy this child actor
					wallsubs[i][j][k]->DestroyComponent();
				}
			}
		}
	}
}


void ALabyrinth::createExit() {
	/// Open and place a mark on the exit
	for (int k = 0; k < 4; k++) {
		if (IsValid(wallsubs[exitX][exitY][k]))
			wallsubs[exitX][exitY][k]->DestroyComponent();
	}
	FVector location(exitX*WALL_SIZE, exitY*WALL_SIZE, 0.0f);
	FRotator rotation(0.0f, 0.0f, 0.0f);
	FActorSpawnParameters spawn_info;
	//Spawn exit mark
	AActor* a = static_cast<AActor*>(GetWorld()->SpawnActor(exit_sign_, &location, &rotation, spawn_info));
}


void ALabyrinth::createPlayerStart() {
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.bNoCollisionFail = true;
	SpawnInfo.Owner = this;
	SpawnInfo.Instigator = NULL;
	SpawnInfo.bDeferConstruction = false;

	FVector Loc(startX*WALL_SIZE, startY*WALL_SIZE, 10.0f);

	GetWorld()->SpawnActor<APlayerStart>(APlayerStart::StaticClass(), Loc, FRotator::ZeroRotator, SpawnInfo);
}


Cell ALabyrinth::getCell(int x, int y) {
	return maze_[x][y];
}

int ALabyrinth::getWallsAt(int x, int y) {
	return getCell(x, y).getWalls();
}