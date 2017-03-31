#include "MazeChase.h"
#include "Engine/TargetPoint.h"
#include "Labyrinth.h"

// Sets default values
ALabyrinth::ALabyrinth(){
	PrimaryActorTick.bCanEverTick = true;

}


void ALabyrinth::OnConstruction(const FTransform& Transform) {
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "CONSTRUCTING MAZE");
	// Generate maze model
	generateModel();
	raiseWalls();

	carveWalls();
	createExit();
	createDoors();
	createPlayerStart();	
}


void ALabyrinth::BeginPlay() {
	Super::BeginPlay();
	spawnMinotaur();

	//Spawn patrol points
	for (int i = 0; i < 3; i++) {
		int x = rand() % ROWS;
		int y = rand() % COLS;

		FVector location(x * WALL_SIZE, y * WALL_SIZE, 0.0f);
		FRotator rotation(0.0f, 0.0f, 0.0f);
		FActorSpawnParameters spawn_info;
		//Spawn actor
		ATargetPoint* point = static_cast<ATargetPoint*>(GetWorld()->SpawnActor(ATargetPoint::StaticClass(), &location, &rotation, spawn_info));
		minos_->patrolList.Push(point);
	}
}


void ALabyrinth::Tick( float DeltaTime ){
	Super::Tick( DeltaTime );

	//if ((int)GetWorld()->GetTimeSeconds() % 5 == 0)
		//rebuild();
}


void ALabyrinth::rebuild() {

	generateModel();
	raiseWalls();
	carveWalls();
	createExit();
	createDoors();
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
	hero_startX = curX;
	hero_startY = curY;

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


/// Creates cells of walls all along the maze
void ALabyrinth::raiseWalls() {
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLS; j++) {
			//WE CREATE 4 WALLS PER CELL.
			for (int k = 0; k < 4; k++) {
				//Create maze's children objects (wall meshes)
				FString name = "ChildWall_" + FString::FromInt(i) + "_" + FString::FromInt(j) + FString::FromInt(k);
				

				FVector location(i*WALL_SIZE, j*WALL_SIZE, 0.0f);
				FRotator rotation(0.0f, 0.0f, 0.0f);
				FActorSpawnParameters spawn_info;
				//Spawn exit mark
				wall_children_meshes_[i][j][k] = static_cast<AActor*>(GetWorld()->SpawnActor(wall_class_, &location, &rotation, spawn_info));


				//wall_children_meshes_[i][j][k] = CreateDefaultSubobject<UStaticMeshComponent>(FName(*name));
				//wall_children_meshes_[i][j][k] = ConstructObject<UStaticMeshComponent>(UStaticMeshComponent::StaticClass(), this, FName(*name));
				AActor* wallsub = wall_children_meshes_[i][j][k];

				//Assign a mesh class to the child mesh component
				//wallsub->SetStaticMesh(wall_class_);
				wallsub->SetActorScale3D(FVector(5.0f, 0.2f, 5.0f));

				//Reposition every wall to form a matrix of walls
				float x = i * WALL_SIZE;
				float y = j * WALL_SIZE;

				//Move the wall from the center of the matrix's cell to its proper location&rotation
				switch (k) {
				case 0:
					wallsub->SetActorRelativeRotation(FQuat(FVector(0.0f, 0.0f, 1.0f), 1.57f));
					wallsub->SetActorRelativeLocation(FVector(x - (WALL_SIZE / 2.0f), y, 0.0f));
					break;
				case 1:
					wallsub->SetActorRelativeLocation(FVector(x, y + (WALL_SIZE / 2.0f), 0.0f));
					break;
				case 2:
					wallsub->SetActorRelativeRotation(FQuat(FVector(0.0f, 0.0f, 1.0f), 1.57f));
					wallsub->SetActorRelativeLocation(FVector(x + (WALL_SIZE / 2.0f), y, 0.0f));
					break;
				case 3:
					wallsub->SetActorRelativeLocation(FVector(x, y - (WALL_SIZE / 2.0f), 0.0f));
					break;
				}
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
					AActor* wallsub = wall_children_meshes_[i][j][k];



					switch (k) {
					case 0:
						if (walls & Cell::WALL_NORTH) {

						}
						else
							wall_children_meshes_[i][j][k]->Destroy();
						break;
					case 1:
						if (walls & Cell::WALL_EAST) {

						}
						else
							wall_children_meshes_[i][j][k]->Destroy();
						break;
					case 2:
						if (walls & Cell::WALL_SOUTH) {

						}
						else
							wall_children_meshes_[i][j][k]->Destroy();
						break;
					case 3:
						if (walls & Cell::WALL_WEST) {

						}
						else
							wall_children_meshes_[i][j][k]->Destroy();
						break;
					}
				}
				else {
					//If there are no walls, destroy this child actor
					wall_children_meshes_[i][j][k]->Destroy();
				}
			}
		}
	}
}


void ALabyrinth::createExit() {
	/// Open and place a mark on the exit
	for (int k = 0; k < 4; k++) {
		if (IsValid(wall_children_meshes_[exitX][exitY][k]))
			wall_children_meshes_[exitX][exitY][k]->Destroy();
	}
	FVector location(exitX*WALL_SIZE, exitY*WALL_SIZE, 0.0f);
	FRotator rotation(0.0f, 0.0f, 0.0f);
	FActorSpawnParameters spawn_info;
	//Spawn exit mark
	AActor* exit = static_cast<AActor*>(GetWorld()->SpawnActor(exit_sign_, &location, &rotation, spawn_info));
}


void ALabyrinth::createDoors() {
	int num_doors = 20;

	for (int i = 0; i < num_doors; i++) {
		int x = rand() % ROWS;
		int y = rand() % COLS;

		int walls = this->getWallsAt(x, y);
		if (walls == Cell::WALL_ALL) {
			continue;
		} else {
			FVector location(x*WALL_SIZE, y*WALL_SIZE, 0.0f);
			FRotator rotation(0.0f, 0.0f, 0.0f);
			FActorSpawnParameters spawn_info;

			//Spawn door
			AActor* door = static_cast<AActor*>(GetWorld()->SpawnActor(door_class_, &location, &rotation, spawn_info));

			x *= WALL_SIZE;
			y *= WALL_SIZE;

			if (!(walls & Cell::WALL_NORTH)) {
				door->SetActorRelativeLocation(FVector(x-(WALL_SIZE / 2.0f), y, 0.0f));
				door->SetActorRelativeRotation(FQuat(FVector(0.0f, 0.0f, 1.0f), 1.57f));
			} else if (!(walls & Cell::WALL_EAST)) {
				door->SetActorRelativeLocation(FVector(x, y+(WALL_SIZE / 2.0f), 0.0f));
			} else if (!(walls & Cell::WALL_SOUTH)) {
				door->SetActorRelativeLocation(FVector(x + (WALL_SIZE / 2.0f), y, 0.0f));
				door->SetActorRelativeRotation(FQuat(FVector(0.0f, 0.0f, 1.0f), 1.57f));
			} else if (!(walls & Cell::WALL_WEST)) {
				door->SetActorRelativeLocation(FVector(x, y-(WALL_SIZE / 2.0f), 0.0f));
			}
		}
	}
}


void ALabyrinth::createPlayerStart() {
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.bNoCollisionFail = true;
	SpawnInfo.Owner = this;
	SpawnInfo.Instigator = NULL;
	SpawnInfo.bDeferConstruction = false;

	FVector Loc(hero_startX*WALL_SIZE, hero_startY*WALL_SIZE, 10.0f);

	GetWorld()->SpawnActor<APlayerStart>(APlayerStart::StaticClass(), Loc, FRotator::ZeroRotator, SpawnInfo);
}


void ALabyrinth::spawnMinotaur() {
	minotaur_startX = minotaur_startY = 10;

	FVector location(minotaur_startX*WALL_SIZE, minotaur_startY*WALL_SIZE, 0.0f);
	FRotator rotation(0.0f, 0.0f, 0.0f);
	FActorSpawnParameters spawn_info;
	//Spawn minotaur
	minos_ = static_cast<AMinotaur*>(GetWorld()->SpawnActor(minotaur_class_, &location, &rotation, spawn_info));
}


Cell ALabyrinth::getCell(int x, int y) {
	return maze_[x][y];
}

int ALabyrinth::getWallsAt(int x, int y) {
	return getCell(x, y).getWalls();
}