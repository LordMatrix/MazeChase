// Marcos Vázquez. ESAT 2017.

#pragma once

#include "GameFramework/Actor.h"
#include "FMODStudioModule.h"
#include "FMOD/fmod_studio.hpp"
#include "../../../../../Plugins/FMODStudio/Source/FMODStudio/Classes/FMODBlueprintStatics.h"
#include "Door.generated.h"

UENUM(BlueprintType)
enum class DoorStatus : uint8 {
	kOpening,
	kClosing,
	kOpen,
	kClosed
};

UCLASS()
class MAZECHASE_API ADoor : public AActor
{
	GENERATED_BODY()
	
public:	
	ADoor();

	virtual void BeginPlay() override;
	
	virtual void Tick( float DeltaSeconds ) override;

	/// Makes the door disappear
	void enable();
	/// Makes the door appear
	void disable();

	/// Opens the door during time_to_stop seconds. Starts closing the door after time_to_close seconds
	UFUNCTION(BlueprintCallable, Category = "PassiveMethods")
	void open(float time_to_stop, float time_to_close);

	/// Sets the door status_ to kClosing
	UFUNCTION(BlueprintCallable, Category = "PassiveMethods")
	void close();

	/// Sets the door status_ to kOpen or kClosed given their current status_
	UFUNCTION(BlueprintCallable, Category = "PassiveMethods")
	void stop();

	/// Sound for door moving and grinding against the walls
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	UFMODEvent* FMOD_grind_event_;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	DoorStatus status_;

private:

	/// Timer handles for managing opening and closing
	FTimerHandle door_stop_opening_handle_;
	FTimerHandle door_close_handle_;
	FTimerHandle door_stop_closing_handle_;

	/// Sound instance to be played
	FFMODEventInstance sound_;
};
