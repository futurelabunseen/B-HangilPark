// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AN/CB_AN_HealActive.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

void UCB_AN_HealActive::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, 
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(MeshComp->GetOwner());
	if (ASC == nullptr)
		return;

	const FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(HealEffectClass, 1.f, ASC->MakeEffectContext());
	ASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
}
