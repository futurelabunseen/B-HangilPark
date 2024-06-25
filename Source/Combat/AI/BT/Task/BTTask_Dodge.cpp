// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Dodge.h"
#include "AIController.h"
#include "Character/CB_EnemyCharacter.h"
#include "AbilitySystemComponent.h"
#include "Data/StateTag.h"

EBTNodeResult::Type UBTTask_Dodge::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACB_EnemyCharacter* Enemy = Cast<ACB_EnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	Enemy->SetWarpTarget();
	UAbilitySystemComponent* ASC = Enemy->GetAbilitySystemComponent();
	if (ASC->HasMatchingGameplayTag(STATE_EQUIPMENT_ON))
	{
		FGameplayTagContainer Container;
		Container.AddTag(STATE_DODGE);
		ASC->TryActivateAbilitiesByTag(Container);

		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
