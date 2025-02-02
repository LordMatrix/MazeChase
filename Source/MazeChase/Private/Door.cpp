// Marcos Vázquez. ESAT 2017.

#include "MazeChase.h"
#include "Door.h"


ADoor::ADoor() {
	PrimaryActorTick.bCanEverTick = true;

	status_ = DoorStatus::kClosed;
}


void ADoor::BeginPlay() {
	Super::BeginPlay();
	
}


void ADoor::Tick( float DeltaTime ) {
	Super::Tick( DeltaTime );

	FVector loc = GetActorLocation();
	bool moving = false;

	switch (status_) {
	case DoorStatus::kOpening:
		//Door goes up
		SetActorLocation(FVector(loc.X, loc.Y, loc.Z - 1));
		moving = true;
		break;
	case DoorStatus::kClosing:
		//Door goes down
		SetActorLocation(FVector(loc.X, loc.Y, loc.Z + 1));
		moving = true;
		break;
	}

	if (moving) {
		//Play a sound effect
		if (!UFMODBlueprintStatics::EventInstanceIsValid(sound_)) {	
			FTransform trans(GetActorLocation());
			sound_ = UFMODBlueprintStatics::PlayEventAtLocation(GetWorld(), FMOD_grind_event_, trans, true);
		}
	} else {
		sound_.Instance->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT);
	}
}


void ADoor::enable() {
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
}


void ADoor::disable() {
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
}


void ADoor::open(float time_to_stop, float time_to_close) {
	if (status_ == DoorStatus::kClosed) {
		status_ = DoorStatus::kOpening;
		
		//Clear timers
		GetWorld()->GetTimerManager().ClearTimer(door_stop_opening_handle_);
		GetWorld()->GetTimerManager().ClearTimer(door_close_handle_);
		GetWorld()->GetTimerManager().ClearTimer(door_stop_closing_handle_);

		//Set new timers
		GetWorld()->GetTimerManager().SetTimer(door_stop_opening_handle_, this, &ADoor::stop, time_to_stop, false);
		GetWorld()->GetTimerManager().SetTimer(door_close_handle_, this, &ADoor::close, time_to_close, false);
		GetWorld()->GetTimerManager().SetTimer(door_stop_closing_handle_, this, &ADoor::stop, time_to_stop + time_to_close, false);
	}
}


void ADoor::close() {
	if (status_ == DoorStatus::kOpen) {
		status_ = DoorStatus::kClosing;
	}
}


void ADoor::stop() {
	status_ = (status_ == DoorStatus::kOpening) ? DoorStatus::kOpen : DoorStatus::kClosed;
}