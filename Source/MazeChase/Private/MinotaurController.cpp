// Fill out your copyright notice in the Description page of Project Settings.

#include "MazeChase.h"
#include "BehaviorTree/BlackboardComponent.h" 
#include "BehaviorTree/BehaviorTree.h"
#include "Minotaur.h"
#include "MinotaurController.h"




AMinotaurController::AMinotaurController() {

	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BBComp"));
	BehaviorTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BTComp"));

}

void AMinotaurController::Possess(APawn* InPawn) {

	Super::Possess(InPawn);

	AMinotaur * MyChar = Cast<AMinotaur>(InPawn);

	if (MyChar) {
		BlackboardComp->InitializeBlackboard(*(MyChar->MyBehavior->BlackboardAsset));
		BehaviorTreeComp->StartTree(*(MyChar->MyBehavior));
	}

}