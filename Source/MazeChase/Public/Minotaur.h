#pragma once

#include "GameFramework/Character.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/PawnSensingComponent.h"
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

	// UPawnSensingComponent Delegates
	UFUNCTION()
		void OnHearNoise(APawn *OtherActor, const FVector &Location, float Volume);
	UFUNCTION()
		void OnSeePawn(APawn *OtherPawn);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
		bool hearing_ = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
		bool seeing_ = false;

	typedef enum {
		kIdle,
		kPatrolling,
		kRoaring,
		kCharging,
		kChasing
	} Status;

	Status status_;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behavior")
		TArray<AActor*> patrolList;

	UFUNCTION()
		AActor* GetNextPatrolPoint();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behavior")
		UBehaviorTree * MyBehavior;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		UPawnSensingComponent* PawnSensing;

	UPROPERTY()
		int currentPatrolPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behavior")
		AActor* player_ref_;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behavior")
		float chase_time_ = 10.0f;

private:
	FTimerHandle seeing_reset_handle_;
	FTimerHandle hearing_reset_handle_;
	FTimerHandle player_ref_reset_handle_;

	void setNoSee() { seeing_ = false; }
	void setNoHear() { hearing_ = false; }
	void setNoPlayerRef() { player_ref_ = nullptr; }
};
