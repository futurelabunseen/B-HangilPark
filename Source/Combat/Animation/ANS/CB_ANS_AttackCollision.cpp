// Fill out your copyright notice in the Description page of Project Settings.

#include "Animation/ANS/CB_ANS_AttackCollision.h"
#include "Character/CB_BaseCharacter.h"
#include "Weapon/CB_Sword.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Abilities/GameplayAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "DrawDebugHelpers.h"

void UCB_ANS_AttackCollision::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
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

void UCB_ANS_AttackCollision::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
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
		bool bResult = UKismetSystemLibrary::BoxTraceSingle(Character,
			Start, End, FVector(15.f), Start.Rotation(),
			UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1),
			false, ActorsToIgnore, EDrawDebugTrace::None, OutHit, true);

		if (bResult && HitActors.Find(OutHit.GetActor()) == ::INDEX_NONE)
		{
			HitActors.Emplace(OutHit.GetActor());
			DoDamage(OutHit);
		}
	}
}

void UCB_ANS_AttackCollision::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (Character)
		Sword->TrailEnd();
}

void UCB_ANS_AttackCollision::DoDamage(FHitResult& HitActor)
{
	FHitResult Hit;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(Sword);
	ActorsToIgnore.Add(Character);

	FVector StartPoint = HitActor.GetActor()->GetActorLocation();
	FVector EndPoint = Character->GetActorLocation();

	UKismetSystemLibrary::LineTraceSingle(Character->GetWorld(), EndPoint, StartPoint,
		UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1),
		true, ActorsToIgnore, EDrawDebugTrace::None, Hit, true);

	FGameplayAbilityTargetDataHandle DataHandle;
	FGameplayAbilityTargetData_SingleTargetHit* TargetData = new FGameplayAbilityTargetData_SingleTargetHit(HitActor);
	DataHandle.Add(TargetData);

	FGameplayEventData Payload;
	Payload.TargetData = DataHandle;
	Payload.EventMagnitude = CheckTheta(Hit.GetActor(), Hit.ImpactPoint);

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitActor.GetActor(), STATE_HIT, Payload);
}

float UCB_ANS_AttackCollision::CheckTheta(AActor* HitActor, FVector& Vector)
{
	const FVector Forward = HitActor->GetActorForwardVector();

	FVector Start = HitActor->GetActorLocation();
	Start.Z = 0.f;
	const FVector ImpactLowered(Vector.X, Vector.Y, Start.Z);
	const FVector ToHit = (ImpactLowered - Start).GetSafeNormal();

	const double CosTheta = FVector::DotProduct(Forward, ToHit);
	double Theta = UKismetMathLibrary::DegAcos(CosTheta);

	FVector Product = FVector::CrossProduct(Forward, ToHit);
	float Sign = UKismetMathLibrary::SignOfFloat(Product.Z);

	Theta *= Sign;
	return Theta;
}
