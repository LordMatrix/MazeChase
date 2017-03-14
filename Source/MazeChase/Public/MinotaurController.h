// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "MinotaurController.generated.h"

/**
 * 
 */
UCLASS()
class MAZECHASE_API AMinotaurController : public AAIController
{
	GENERATED_BODY()
	
public:

	AMinotaurController();

	virtual void Possess(APawn* InPawn) override;

	UPROPERTY(EditAnywhere, Category = Behaviour)
		UBlackboardComponent* BlackboardComp;

	UPROPERTY(EditAnywhere, Category = Behaviour)
		UBehaviorTreeComponent* BehaviorTreeComp;
	
	
};
