// Fill out your copyright notice in the Description page of Project Settings.

#include "CB_ANS_Activate.h"
#include "Character/CB_BaseCharacter.h"
#include "Weapon/CB_Sword.h"

#include "Kismet/GameplayStatics.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "DrawDebugHelpers.h"

// 수정 필요
void UCB_ANS_Activate::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	HitActors.Empty();

	const ACB_BaseCharacter* Character = Cast<ACB_BaseCharacter>(MeshComp->GetOwner());
	if (Character)
	{
		ACB_Sword* Sword = Cast<ACB_Sword>(Character->GetWeapon());
		Sword->TrailStart();
	}
}

void UCB_ANS_Activate::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, 
	float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	ACB_BaseCharacter* Character = Cast<ACB_BaseCharacter>(MeshComp->GetOwner());
	if (Character)
	{
		ACB_Sword* Sword = Cast<ACB_Sword>(Character->GetWeapon());
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
		{
			HitActors.AddUnique(OutHit.GetActor());
			//DrawDebugSphere(Character->GetWorld(), OutHit.ImpactPoint, 10.f, 5, FColor::Orange, true, 2.f);
		}
	}
}

void UCB_ANS_Activate::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	const ACB_BaseCharacter* Character = Cast<ACB_BaseCharacter>(MeshComp->GetOwner());
	if (Character)
	{
		ACB_Sword* Sword = Cast<ACB_Sword>(Character->GetWeapon());
		Sword->TrailEnd();
	}

	for (const auto& HitActor : HitActors)
	{
		FGameplayEventData Payload;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitActor, STATE_HIT, Payload);
	}
}
