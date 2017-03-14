#include "MazeChase.h"
#include "Minotaur.h"


// Sets default values
AMinotaur::AMinotaur() {
	PrimaryActorTick.bCanEverTick = true;
	status_ = kIdle;
}


void AMinotaur::BeginPlay() {
	Super::BeginPlay();
}


void AMinotaur::Tick( float DeltaTime ) {
	Super::Tick( DeltaTime );
	updateFSM();
}


void AMinotaur::SetupPlayerInputComponent(class UInputComponent* InputComponent) {
	Super::SetupPlayerInputComponent(InputComponent);
}


void AMinotaur::senseEnvironment() {

}


void AMinotaur::updateFSM() {
	switch (status_) {
		case kIdle:
			break;
		case kPatrolling:
			break;
		case kRoaring:
			break;
		case kCharging:
			break;
		case kChasing:
			break;
	}
}