// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BT/BTService_IsStun.h"
#include "AIController.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Tags/StateTag.h"
//#include "BehaviorTree/BTFunctionLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"

void UBTService_IsStun::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* OwningPawn = OwnerComp.GetAIOwner()->GetPawn();
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OwningPawn);
	bool bIsStunned = ASC->HasMatchingGameplayTag(STATE_DEBUFF_STUN);

	OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("IsStunned"), bIsStunned);
}
