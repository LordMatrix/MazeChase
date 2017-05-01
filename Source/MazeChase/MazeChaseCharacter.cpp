// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "MazeChase.h"
#include "Kismet/HeadMountedDisplayFunctionLibrary.h"

#include "DrawDebugHelpers.h"

#include "MazeChaseCharacter.h"

//////////////////////////////////////////////////////////////////////////
// AMazeChaseCharacter

AMazeChaseCharacter::AMazeChaseCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	//Status default values
	pawn_dead_ = false;
	pawn_sneaking_ = false;
	pawn_buttoning_ = false;
	pawn_shouting_ = false;

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	// Create noise emiter component
	noise_emiter_ = CreateDefaultSubobject<UPawnNoiseEmitterComponent>(TEXT("NoiseEmiter"));

	//Create torchlight
	torchlight_ = CreateDefaultSubobject<UChildActorComponent>(TEXT("Torchlight"));
	torchlight_->SetupAttachment(GetMesh(), FName("headSocket"));
	static ConstructorHelpers::FClassFinder<AActor> torchlight_class(TEXT("/Game/Blueprints/BP_Torchlight"));
	torchlight_->SetChildActorClass(torchlight_class.Class);
	torchlight_->RelativeScale3D = FVector(0.1f, 0.1f, 0.1f);
	torchlight_->RelativeLocation = FVector(10.0f, 10.0f, 0.0f);
	torchlight_->RelativeRotation = FRotator(0.0f, 90.0f, 0.0f);

	nervousness_ = 0.0f;
	max_nervousness_ = 10.0f;
}


void AMazeChaseCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	
	// Raycast looking for doors
	FHitResult out;
	FVector ray_start = this->GetActorLocation();
	FVector frwd = this->GetActorForwardVector();
	FVector ray_end = ray_start + frwd*100.0f;
	GetWorld()->LineTraceSingleByChannel(out, ray_start, ray_end, ECollisionChannel::ECC_Visibility);

	DrawDebugLine(GetWorld(), ray_start, ray_end, FColor::Red);

	if (out.bBlockingHit && out.GetActor()->GetName().Contains("BP_DoorButton")) {
		button_ = Cast<ADoorButton>(out.GetActor());
	} else {
		button_ = nullptr;
	}

	//Sanity checks
	if (pawn_running_ || pawn_shouting_) {
		this->getScared(0.01f);
	} else {
		if (nervousness_ > 0.0f)
			nervousness_ -= 0.01f;
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void AMazeChaseCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMazeChaseCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMazeChaseCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AMazeChaseCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMazeChaseCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AMazeChaseCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AMazeChaseCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AMazeChaseCharacter::OnResetVR);

	//Me-defined actions
	PlayerInputComponent->BindAction("Fly", IE_Pressed, this, &AMazeChaseCharacter::OnInteractInputPressed);
	PlayerInputComponent->BindAction("Fly", IE_Released, this, &AMazeChaseCharacter::OnInteractInputReleased);

	PlayerInputComponent->BindAction("Sneak", IE_Pressed, this, &AMazeChaseCharacter::OnSneakInputPressed);
	PlayerInputComponent->BindAction("Sneak", IE_Released, this, &AMazeChaseCharacter::OnSneakInputReleased);

	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &AMazeChaseCharacter::OnRunInputPressed);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &AMazeChaseCharacter::OnRunInputReleased);

	PlayerInputComponent->BindAction("Shout", IE_Pressed, this, &AMazeChaseCharacter::OnShoutInputPressed);
	PlayerInputComponent->BindAction("Shout", IE_Released, this, &AMazeChaseCharacter::OnShoutInputReleased);

	PlayerInputComponent->BindAction("TorchLight", IE_Pressed, this, &AMazeChaseCharacter::OnTorchlightInputPressed);
	PlayerInputComponent->BindAction("TorchLight", IE_Released, this, &AMazeChaseCharacter::OnTorchlightInputReleased);
}


void AMazeChaseCharacter::OnInteractInputPressed() {
	if (IsValid(button_)) {
		pawn_buttoning_ = true;
	} else {
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
	}
}


void AMazeChaseCharacter::OnInteractInputReleased() {
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}


void AMazeChaseCharacter::OnSneakInputPressed() {
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	pawn_sneaking_ = true;
	step_loudness_ = 1.0f;
}


void AMazeChaseCharacter::OnSneakInputReleased() {
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	pawn_sneaking_ = false;
	step_loudness_ = 2.0f;
}


void AMazeChaseCharacter::OnRunInputPressed() {
	if (!pawn_sneaking_) {
		pawn_running_ = true;
		GetCharacterMovement()->MaxWalkSpeed = 900.0f;
		step_loudness_ = 3.0f;
	}
}


void AMazeChaseCharacter::OnRunInputReleased() {
	if (!pawn_sneaking_) {
		pawn_running_ = false;
		GetCharacterMovement()->MaxWalkSpeed = 600.0f;
		step_loudness_ = 2.0f;
	}
}


void AMazeChaseCharacter::OnShoutInputPressed() {
	pawn_shouting_ = true;
}


void AMazeChaseCharacter::OnShoutInputReleased() {
	pawn_shouting_ = false;
}


void AMazeChaseCharacter::OnTorchlightInputPressed() {
	torchlight_->SetVisibility(!torchlight_->bVisible, true);
}


void AMazeChaseCharacter::OnTorchlightInputReleased() {
}


void AMazeChaseCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AMazeChaseCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	// jump, but only on the first touch
	if (FingerIndex == ETouchIndex::Touch1)
	{
		Jump();
	}
}

void AMazeChaseCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	if (FingerIndex == ETouchIndex::Touch1)
	{
		StopJumping();
	}
}

void AMazeChaseCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMazeChaseCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AMazeChaseCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AMazeChaseCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}



void AMazeChaseCharacter::getScared(float amount) {
	if (nervousness_ < max_nervousness_)
		nervousness_ += amount;
}