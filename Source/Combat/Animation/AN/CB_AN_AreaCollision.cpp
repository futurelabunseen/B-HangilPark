// Fill out your copyright notice in the Description page of Project Settings.

#include "Animation/AN/CB_AN_AreaCollision.h"
#include "Character/CB_BaseCharacter.h"
#include "Weapon/CB_Sword.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "DrawDebugHelpers.h"

void UCB_AN_AreaCollision::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	Character = Cast<ACB_BaseCharacter>(MeshComp->GetOwner());
	if (Character)
	{
		ACB_Sword* Sword = Cast<ACB_Sword>(Character->GetWeapon());
		TArray<FHitResult> OutHits;
		TArray<AActor*> ActorsToIgnore;
		ActorsToIgnore.Add(Sword);
		ActorsToIgnore.Add(Character);

		FVector Origin = MeshComp->GetSocketLocation("root");
		UKismetSystemLibrary::SphereTraceMulti(Character, Origin, Origin, SkillRadius, 
			UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1),
			false, ActorsToIgnore, EDrawDebugTrace::None, OutHits, true);

		DrawDebugSphere(Character->GetWorld(), Origin, SkillRadius, 20, FColor::Orange, false, 1.f);

		TArray<AActor*> HitResults;
		for (auto OutHit : OutHits)
		{
			if (HitResults.Find(OutHit.GetActor()) == ::INDEX_NONE)
				HitResults.Emplace(OutHit.GetActor());
		}

		DoDamage(HitResults, ActorsToIgnore);
	}
}

void UCB_AN_AreaCollision::DoDamage(TArray<AActor*> HitResults, TArray<AActor*> ActorsToIgnore)
{
	FHitResult Hit;
	const FVector StartPoint = Character->GetActorLocation();
	for (auto HitResult : HitResults)
	{
		FVector EndPoint = HitResult->GetActorLocation();

		UKismetSystemLibrary::LineTraceSingle(Character->GetWorld(), StartPoint, EndPoint,
			UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1),
			true, ActorsToIgnore, EDrawDebugTrace::None, Hit, true);

		FGameplayAbilityTargetDataHandle DataHandle;
		FGameplayAbilityTargetData_SingleTargetHit* TargetData = new FGameplayAbilityTargetData_SingleTargetHit(Hit);
		DataHandle.Add(TargetData);

		FGameplayEventData Payload;
		Payload.TargetData = DataHandle;
		Payload.EventMagnitude = CheckTheta(Hit.GetActor(), Hit.ImpactPoint);

		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Hit.GetActor(), GameplayTag, Payload);
	}
}

float UCB_AN_AreaCollision::CheckTheta(AActor* HitActor, FVector& Vector)
{
	FVector Forward = HitActor->GetActorForwardVector();

	FVector Start = HitActor->GetActorLocation();
	Start.Z = 0.f;
	FVector ImpactLowered(Vector.X, Vector.Y, Start.Z);
	FVector ToHit = (ImpactLowered - Start).GetSafeNormal();

	double CosTheta = FVector::DotProduct(Forward, ToHit);
	double Theta = UKismetMathLibrary::DegAcos(CosTheta);

	FVector Product = FVector::CrossProduct(Forward, ToHit);
	float Sign = UKismetMathLibrary::SignOfFloat(Product.Z);

	Theta *= Sign;
	return Theta;
}
