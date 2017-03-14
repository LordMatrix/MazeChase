// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_GetNextPatrolTarget.generated.h"

/**
*
*/
UCLASS()
class MAZECHASE_API UBTTask_GetNextPatrolTarget : public UBTTaskNode
{
	GENERATED_BODY()

public:

	/** Execute task ONCE */
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp,
		uint8* NodeMemory) override;


};
