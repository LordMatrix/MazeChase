#include "MazeChase.h"
#include "Minotaur.h"


// Sets default values
AMinotaur::AMinotaur() {
	PrimaryActorTick.bCanEverTick = true;
	status_ = kIdle;
	currentPatrolPoint = -1;

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	PawnSensing->SetPeripheralVisionAngle(25.f);

	PawnSensing->OnSeePawn.AddDynamic(this, &AMinotaur::OnSeePawn);
	PawnSensing->OnHearNoise.AddDynamic(this, &AMinotaur::OnHearNoise);
}


void AMinotaur::BeginPlay() {
	Super::BeginPlay();
}


void AMinotaur::Tick( float DeltaTime ) {
	Super::Tick( DeltaTime );
}


void AMinotaur::SetupPlayerInputComponent(class UInputComponent* InputComponent) {
	Super::SetupPlayerInputComponent(InputComponent);
}


void AMinotaur::OnHearNoise(APawn *OtherActor, const FVector &Location, float Volume) {
	hearing_ = true;
	player_ref_ = OtherActor;

	//Reset hearing_ status after a brief while
	GetWorld()->GetTimerManager().ClearTimer(hearing_reset_handle_);
	GetWorld()->GetTimerManager().SetTimer(hearing_reset_handle_, this, &AMinotaur::setNoHear, 3.0f, false);

	//Reset player reference after a short while
	GetWorld()->GetTimerManager().ClearTimer(player_ref_reset_handle_);
	GetWorld()->GetTimerManager().SetTimer(player_ref_reset_handle_, this, &AMinotaur::setNoPlayerRef, chase_time_, false);
}


void AMinotaur::OnSeePawn(APawn *OtherPawn) {
	seeing_ = true;
	player_ref_ = OtherPawn;

	//Reset seeing_ status after a brief while
	GetWorld()->GetTimerManager().ClearTimer(seeing_reset_handle_);
	GetWorld()->GetTimerManager().SetTimer(seeing_reset_handle_, this, &AMinotaur::setNoSee, 3.0f, false);

	//Reset player reference after a short while
	GetWorld()->GetTimerManager().ClearTimer(player_ref_reset_handle_);
	GetWorld()->GetTimerManager().SetTimer(player_ref_reset_handle_, this, &AMinotaur::setNoPlayerRef, chase_time_, false);
}


AActor* AMinotaur::GetNextPatrolPoint() {

	//AActor * TmpTarget = patrolList[currentPatrolPoint];

	if (currentPatrolPoint >= patrolList.Num() - 1) {

		currentPatrolPoint = 0;
	}
	else {
		currentPatrolPoint++;
	}


	return patrolList[currentPatrolPoint];

}