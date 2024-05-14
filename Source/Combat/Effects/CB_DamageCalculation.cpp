// Fill out your copyright notice in the Description page of Project Settings.


#include "Effects/CB_DamageCalculation.h"
#include "Attribute/CB_CharacterAttributeSet.h"
#include "Tags/StateTag.h"

UCB_DamageCalculation::UCB_DamageCalculation()
{

}

void UCB_DamageCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);

	UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	if (SourceASC && TargetASC)
	{
		AActor* SourceActor = SourceASC->GetAvatarActor();
		AActor* TargetActor = TargetASC->GetAvatarActor();

		if (SourceActor && TargetActor)
		{
			/*UE_LOG(LogTemp, Warning, TEXT("Source Actor : %s"), *SourceActor->GetName());
			UE_LOG(LogTemp, Warning, TEXT("Target Actor : %s"), *TargetActor->GetName());*/

			float Damage = (SourceASC->HasMatchingGameplayTag(STATE_GUARD)) ? 10.f : 30.f;

			OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(UCB_CharacterAttributeSet::GetHealthAttribute(), EGameplayModOp::Additive, -Damage));
		}
	}
}
