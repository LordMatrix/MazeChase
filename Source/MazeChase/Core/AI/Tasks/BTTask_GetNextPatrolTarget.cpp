// Fill out your copyright notice in the Description page of Project Settings.

#include "MazeChase.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "Minotaur.h"
#include "BTTask_GetNextPatrolTarget.h"


EBTNodeResult::Type UBTTask_GetNextPatrolTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {

	//return EBTNodeResult::InProgress;

	AAIController* MyController = OwnerComp.GetAIOwner();

	if (!MyController)
		return EBTNodeResult::Failed;

	//UBlackboardComponent* BBComp = MyController->BlackboardComp;
	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	if (!BBComp)
		return EBTNodeResult::Failed;

	AMinotaur* myChar = Cast<AMinotaur>(MyController->GetControlledPawn());


	BBComp->SetValueAsObject("TargetPatrol", myChar->GetNextPatrolPoint());

	return EBTNodeResult::Succeeded;

}

