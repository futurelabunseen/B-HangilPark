// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AN/CB_AN_FootStep.h"
#include "GameFramework/Character.h"
#include "DrawDebugHelpers.h"

void UCB_AN_FootStep::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	ACharacter* Character = Cast<ACharacter>(MeshComp->GetOwner());
	if (Character)
	{
		FHitResult HitResult;
		const FVector Start = Character->GetMesh()->GetSocketLocation(SocketName); 
		const FVector End = Start + FVector(0.f, 0.f, -400.f);

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(Character);
		QueryParams.bTraceComplex = true;
		QueryParams.bReturnPhysicalMaterial = true;

		MeshComp->GetWorld()->LineTraceSingleByChannel(HitResult, Start, End,
			ECollisionChannel::ECC_Visibility, QueryParams);
		if (HitResult.GetActor())
			MatName = HitResult.GetActor()->GetName();
	}
}
