// Marcos Vázquez. ESAT 2017.

#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_UpdateSensoryInfo.generated.h"

/**
 * 
 */
UCLASS()
class MAZECHASE_API UBTTask_UpdateSensoryInfo : public UBTTaskNode
{
	GENERATED_BODY()
	
public:

	/** Execute task ONCE */
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp,
		uint8* NodeMemory) override;
};
