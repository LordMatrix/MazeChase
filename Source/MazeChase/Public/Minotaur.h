#pragma once

#include "GameFramework/Character.h"
#include "Minotaur.generated.h"

UCLASS()
class MAZECHASE_API AMinotaur : public ACharacter
{
	GENERATED_BODY()

public:
	AMinotaur();
	virtual void BeginPlay() override;
	virtual void Tick( float DeltaSeconds ) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	void updateFSM();
	void senseEnvironment();

	typedef enum {
		kIdle,
		kPatrolling,
		kRoaring,
		kCharging,
		kChasing
	} Status;

	Status status_;
};
