// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/CB_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

ACB_AIController::ACB_AIController()
{
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
}

void ACB_AIController::StopAI()
{
	if (IsValid(BehaviorTreeComponent))
		BehaviorTreeComponent->StopTree();
}
