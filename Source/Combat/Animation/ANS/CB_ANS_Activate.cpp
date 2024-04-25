// Fill out your copyright notice in the Description page of Project Settings.

#include "CB_ANS_Activate.h"
#include "Character/CB_BaseCharacter.h"
#include "Weapon/CB_Sword.h"

#include "Kismet/GameplayStatics.h"
#include "Abilities/GameplayAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "DrawDebugHelpers.h"

void UCB_ANS_Activate::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	HitActors.Empty();
	Character = Cast<ACB_BaseCharacter>(MeshComp->GetOwner());
	if (Character)
	{
		Sword = Cast<ACB_Sword>(Character->GetWeapon());
		Sword->TrailStart();
	}
}

void UCB_ANS_Activate::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, 
	float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	if (Character)
	{
		FHitResult OutHit;
		TArray<AActor*> ActorsToIgnore;
		ActorsToIgnore.Add(Sword);
		ActorsToIgnore.Add(Character);

		const FVector Start = Sword->GetWeaponMesh()->GetSocketLocation("StartSocket");
		const FVector End = Sword->GetWeaponMesh()->GetSocketLocation("EndSocket");

		// ForDuration, None
		bool bResult = UKismetSystemLibrary::BoxTraceSingle(Sword,
			Start, End, FVector(5.f), Start.Rotation(),
			UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1),
			false, ActorsToIgnore, EDrawDebugTrace::None, OutHit, true);

		if (bResult)
			HitActors.Add(OutHit.GetActor()->GetName(), OutHit);
	}
}

void UCB_ANS_Activate::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (Character)
		Sword->TrailEnd();

	for (const auto& HitActor : HitActors)
	{
		// UE_LOG(LogTemp, Warning, TEXT("Actor : %s, HitResult : %f, %f, %f"), *HitActor.Key, HitActor.Value.ImpactPoint.X, HitActor.Value.ImpactPoint.Y, HitActor.Value.ImpactPoint.Z);
		// DrawDebugSphere(Character->GetWorld(), HitActor.Value.ImpactPoint, 10.f, 5, FColor::Orange, false, 1.f);
		FHitResult HitResult = HitActor.Value;
		FGameplayAbilityTargetDataHandle DataHandle;
		FGameplayAbilityTargetData_SingleTargetHit* TargetData = new FGameplayAbilityTargetData_SingleTargetHit(HitResult);
		DataHandle.Add(TargetData);

		FGameplayEventData Payload;
		Payload.TargetData = DataHandle;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitResult.GetActor(), STATE_HIT, Payload);
	}
}
