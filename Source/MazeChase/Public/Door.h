// Marcos Vázquez. ESAT 2017.

#pragma once

#include "GameFramework/Actor.h"
#include "Door.generated.h"

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
};
