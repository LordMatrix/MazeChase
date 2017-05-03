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

	void enable();
	void disable();

	UFUNCTION(BlueprintCallable, Category = "PassiveMethods")
	void open(float time_to_stop, float time_to_close);
	UFUNCTION(BlueprintCallable, Category = "PassiveMethods")
	void close();
	UFUNCTION(BlueprintCallable, Category = "PassiveMethods")
	void stop();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	UFMODEvent* fevent_;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	DoorStatus status_;

private:
	FTimerHandle door_stop_opening_handle_;
	FTimerHandle door_close_handle_;
	FTimerHandle door_stop_closing_handle_;

	FFMODEventInstance sound_;
};
