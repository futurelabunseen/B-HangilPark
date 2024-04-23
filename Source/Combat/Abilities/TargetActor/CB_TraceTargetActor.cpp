// Fill out your copyright notice in the Description page of Project Settings.

#include "Abilities/TargetActor/CB_TraceTargetActor.h"
#include "Abilities/GameplayAbility.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"
#include "Interface/CB_CombatInterface.h"
#include "Kismet/KismetSystemLibrary.h"

void ACB_TraceTargetActor::StartTargeting(UGameplayAbility* Ability)
{
	Super::StartTargeting(Ability);
	SourceActor = Ability->GetCurrentActorInfo()->AvatarActor.Get();
}

void ACB_TraceTargetActor::ConfirmTargetingAndContinue()
{
	if (SourceActor)
	{
		FGameplayAbilityTargetDataHandle DataHandle = MakeTargetData();
		TargetDataReadyDelegate.Broadcast(DataHandle);
	}
}

FGameplayAbilityTargetDataHandle ACB_TraceTargetActor::MakeTargetData() const
{
	ACharacter* Character = CastChecked<ACharacter>(SourceActor);
	ICB_CombatInterface* Interface = Cast<ICB_CombatInterface>(Character);

	const FVector Start = Interface->GetWeaponSocketLocation("StartSocket");
	const FVector End = Interface->GetWeaponSocketLocation("EndSocket");

	FHitResult HitResult;
	ETraceTypeQuery MyTraceType = UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1);
	
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(Character);
	// EDrawDebugTrace::ForDuration / None
	bool HitDetected = UKismetSystemLibrary::BoxTraceSingle(this, Start, End, FVector(10.f),
		Start.Rotation(), MyTraceType, false, ActorsToIgnore, EDrawDebugTrace::ForDuration, HitResult, true);

	FGameplayAbilityTargetDataHandle DataHandle;
	if (HitDetected)
	{
		FGameplayAbilityTargetData_SingleTargetHit* TargetData = new FGameplayAbilityTargetData_SingleTargetHit(HitResult);
		DataHandle.Add(TargetData);
	}

	return DataHandle;	
}