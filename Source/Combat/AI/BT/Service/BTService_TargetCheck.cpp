// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_TargetCheck.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Data/StateTag.h"
#include "Data/EnemyState.h"

void UBTService_TargetCheck::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	APawn* OwningPawn = OwnerComp.GetAIOwner()->GetPawn();
	const FName& KeyName = TEXT("EnemyState");
	uint8 EnumValue;
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OwningPawn);

	
	if (ASC->HasMatchingGameplayTag(STATE_ATTACK_LIGHT))
		EnumValue = (uint8)EEnemyState::EES_Guard;
	else if (ASC->HasMatchingGameplayTag(STATE_ATTACK_SKILL_Q) || 
		ASC->HasMatchingGameplayTag(STATE_ATTACK_SKILL_E))
		EnumValue = (uint8)EEnemyState::EES_Dodge;
	else
		EnumValue = (uint8)EEnemyState::EES_Attack;

	OwnerComp.GetBlackboardComponent()->SetValueAsEnum(KeyName, EnumValue);
}
