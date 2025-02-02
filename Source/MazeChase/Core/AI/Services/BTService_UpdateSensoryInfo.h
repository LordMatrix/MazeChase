// Marcos Vázquez. ESAT 2017.

#pragma once

#include "BehaviorTree/BTService.h"
#include "BTService_UpdateSensoryInfo.generated.h"

/**
 * 
 */
UCLASS()
class MAZECHASE_API UBTService_UpdateSensoryInfo : public UBTService
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = Blackboard)
		FBlackboardKeySelector TargetObject;

protected:

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	/** initialize any asset related data */
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

};
