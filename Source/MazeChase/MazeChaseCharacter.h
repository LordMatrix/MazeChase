// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "Public/DoorButton.h"
#include "MazeChaseCharacter.generated.h"

UCLASS(config=Game)
class AMazeChaseCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	AMazeChaseCharacter();

	virtual void Tick(float DeltaSeconds) override;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = References)
		ADoorButton* button_;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Components)
		UPawnNoiseEmitterComponent* noise_emiter_;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components)
		UChildActorComponent* torchlight_;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Config)
		float step_loudness_ = 2.0f;


protected:

	//Custom callbacks
	void OnInteractInputPressed();
	void OnInteractInputReleased();
	void OnSneakInputPressed();
	void OnSneakInputReleased();
	void OnRunInputPressed();
	void OnRunInputReleased();
	void OnShoutInputPressed();
	void OnShoutInputReleased();
	void OnTorchlightInputPressed();
	void OnTorchlightInputReleased();

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION(BlueprintCallable, Category = "PassiveMethods")
		void getScared(float amount);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	bool pawn_dead_;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	bool pawn_sneaking_;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	bool pawn_buttoning_;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
		bool pawn_shouting_;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
		bool pawn_running_;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
		float nervousness_;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
		float max_nervousness_;
};

