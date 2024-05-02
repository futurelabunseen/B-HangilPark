// Fill out your copyright notice in the Description page of Project Settings.

#include "CB_DodgeAbility.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Kismet/KismetMathLibrary.h"

UCB_DodgeAbility::UCB_DodgeAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UCB_DodgeAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, 
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, 
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	CommitAbility(Handle, ActorInfo, ActivationInfo);

	Character = CastChecked<ACharacter>(ActorInfo->AvatarActor.Get());

	UAbilityTask_PlayMontageAndWait* PlayDodgeTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this, NAME_None, DodgeMontage, 1.f, CheckSectionName(CheckTheta()));
	PlayDodgeTask->OnCompleted.AddUniqueDynamic(this, &UCB_DodgeAbility::OnCompleteCallback);
	PlayDodgeTask->ReadyForActivation();
}

void UCB_DodgeAbility::OnCompleteCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

float UCB_DodgeAbility::CheckTheta()
{
	const FVector Forward = Character->GetActorForwardVector();
	FVector End = (Character->GetCharacterMovement()->Velocity).GetSafeNormal();
	End.Z = 0.f;

	const double CosTheta = FVector::DotProduct(Forward, End);
	double Theta = UKismetMathLibrary::DegAcos(CosTheta);

	FVector Product = FVector::CrossProduct(Forward, End);
	float Sign = UKismetMathLibrary::SignOfFloat(Product.Z);

	Theta *= Sign;
	return Theta;
}

FName UCB_DodgeAbility::CheckSectionName(const float Theta)
{
	// ������ �� �� ���� �ʱ�ȭ, MakeTuple(Min, Max)�ε� ����
	static const FData Sections[] = {
		{ MakeTuple(-22.5, 22.5), FName("Fwd")},
		{ MakeTuple(22.5, 67.5), FName("FR")},
		{ MakeTuple(67.5, 112.5), FName("Right")},
		{ MakeTuple(112.5, 157.5), FName("BR")},
		{ MakeTuple(-67.5, -22.5), FName("FL")},
		{ MakeTuple(-112.5, -67.5), FName("Left")},
		{ MakeTuple(-157.5, -112.5), FName("BL")}
	};

	for (const auto& Section : Sections)
		if (Section.Range.Key <= Theta && Theta < Section.Range.Value)
			return Section.Name;

	return FName("Bwd");
}
