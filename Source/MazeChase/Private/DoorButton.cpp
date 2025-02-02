// Marcos Vázquez. ESAT 2017.

#include "MazeChase.h"
#include "DoorButton.h"


ADoorButton::ADoorButton() {
	time_to_stop_ = 5.0f;
	time_to_close_ = 10.0f;
}


void ADoorButton::BeginPlay() {
	Super::BeginPlay();
}


void ADoorButton::Tick( float DeltaTime ) {
	Super::Tick( DeltaTime );
}


void ADoorButton::openDoors() {
	for (int i = 0; i < linked_doors_.Num(); i++) {
		ADoor* door = linked_doors_[i];
		//door->disable();
		door->open(time_to_stop_, time_to_close_);
	}
}