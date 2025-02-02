// Marcos Vázquez. ESAT 2017.

#pragma once

#include "BehaviorTree/BTService.h"
#include "FMODStudioModule.h"
#include "FMOD/fmod_studio.hpp"
#include "BTService_PlayFMODEvent.generated.h"

/**
 * 
 */
UCLASS()
class MAZECHASE_API UBTService_PlayFMODEvent : public UBTService
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, Category = Blackboard)
		FBlackboardKeySelector TargetObject;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Audio)
		UFMODEvent* TestEvent;

protected:

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	/** initialize any asset related data */
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	
	
	
};
