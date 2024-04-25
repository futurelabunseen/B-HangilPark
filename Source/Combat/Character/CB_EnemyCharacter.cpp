// Fill out your copyright notice in the Description page of Project Settings.

#include "CB_EnemyCharacter.h"
#include "AbilitySystemComponent.h"
#include "Attribute/CB_CharacterAttributeSet.h"

ACB_EnemyCharacter::ACB_EnemyCharacter()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	AttributeSet = CreateDefaultSubobject<UCB_CharacterAttributeSet>(TEXT("AttributeSet"));

}

void ACB_EnemyCharacter::SetOutLine(bool bIsShow)
{
	GetMesh()->SetRenderCustomDepth(bIsShow);
}

void ACB_EnemyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	ASC->InitAbilityActorInfo(this, this); 

	FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(InitStatEffect, Level, EffectContextHandle);
	if (EffectSpecHandle.IsValid())
		ASC->BP_ApplyGameplayEffectSpecToSelf(EffectSpecHandle);
	
	for (const auto& Ability : Abilities)
	{
		FGameplayAbilitySpec Spec(Ability);
		ASC->GiveAbility(Spec);
	}

}
