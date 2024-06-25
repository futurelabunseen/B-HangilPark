// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_Guard.h"
#include "AIController.h"
#include "Character/CB_EnemyCharacter.h"

EBTNodeResult::Type UBTTask_Guard::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACB_EnemyCharacter* Enemy = Cast<ACB_EnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (Enemy)
	{
		Enemy->SetWarpTarget();
		Enemy->SetIsGuard(true);
		return EBTNodeResult::Succeeded;
	}
	else
		return EBTNodeResult::Failed;
	
}
