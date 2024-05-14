// Fill out your copyright notice in the Description page of Project Settings.

#include "CB_TrailComponent.h"
#include "Actors/CB_Trail.h"
#include "Character/CB_BaseCharacter.h"
//#include "GameFramework/Character.h"

UCB_TrailComponent::UCB_TrailComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UCB_TrailComponent::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast<ACB_BaseCharacter>(GetOwner());
}

void UCB_TrailComponent::StartTrail(FGameplayTag Tag)
{
	MakeTrail();

	if (Character->HasGameplayTag(Tag))
	{
		static FTimerHandle TrailTimerHandle;
		FTimerDelegate TimerDel;
		TimerDel.BindUFunction(this, FName(TEXT("TrailTimerReset")), Tag);
		GetWorld()->GetTimerManager().SetTimer(TrailTimerHandle, TimerDel, TimerRate::AutomaticTrailRate, false);
	}
}

void UCB_TrailComponent::TrailTimerReset(FGameplayTag Tag)
{
	StartTrail(Tag);
}

void UCB_TrailComponent::MakeTrail()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = Character;
	FRotator rotator = Character->GetActorRotation();
	FVector  SpawnLocation = Character->GetActorLocation();
	SpawnLocation.Z -= 90;
	rotator.Yaw -= 90;
	auto GTrail = Cast<ACB_Trail>(GetWorld()->SpawnActor<AActor>(
		ActorToSpawn, SpawnLocation, rotator, SpawnParams));
	
	if (GTrail)
		GTrail->Init(Character->GetMesh());
}

void UCB_TrailComponent::SprintTrail()
{
	if (!bTrail) 
		return;

	bTrail = false;
	static FTimerHandle SprintTimerHandle;

	GetWorld()->GetTimerManager().SetTimer(SprintTimerHandle, [this]() {
		if (bTrail == false) 
		{
			GetWorld()->GetTimerManager().ClearTimer(SprintTimerHandle);
			SprintTrail();
			bTrail = true;
			MakeTrail();
		}
		}, TimerRate::SprintRate, true);
}