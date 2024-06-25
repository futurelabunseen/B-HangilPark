// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ResetState.h"
#include "AIController.h"
#include "Character/CB_EnemyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTTask_ResetState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACB_EnemyCharacter* Enemy = Cast<ACB_EnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if(Enemy)
	{
		Enemy->SetIsGuard(false);
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(FName("EnemyState"), (uint8)EEnemyState::EES_Idle);
		return EBTNodeResult::Succeeded;

	}

	return EBTNodeResult::Failed;
}
