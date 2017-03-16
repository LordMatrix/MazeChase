// Marcos Vázquez. ESAT 2017.

#include "MazeChase.h"
#include "Door.h"


ADoor::ADoor() {
	PrimaryActorTick.bCanEverTick = true;
}


void ADoor::BeginPlay() {
	Super::BeginPlay();
	
}


void ADoor::Tick( float DeltaTime ) {
	Super::Tick( DeltaTime );
}


void ADoor::enable() {
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
}


void ADoor::disable() {
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
}