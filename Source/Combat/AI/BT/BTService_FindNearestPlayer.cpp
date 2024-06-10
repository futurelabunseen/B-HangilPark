// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BT/BTService_FindNearestPlayer.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BTFunctionLibrary.h"

void UBTService_FindNearestPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	APawn* OwningPawn = AIOwner->GetPawn();
	const FName TargetTag = OwningPawn->ActorHasTag(FName("Player")) ? FName("Enemy") : FName("Player");

	TArray<AActor*> ActorsWithTag;
	UGameplayStatics::GetAllActorsWithTag(OwningPawn, TargetTag, ActorsWithTag);

	float ClosetDistance = TNumericLimits<float>::Max();
	AActor* ClosetActor = nullptr;
	for (AActor* Actor : ActorsWithTag)
	{
		//GEngine->AddOnScreenDebugMessage(2, .5f, FColor::Orange, *Actor->GetName());
		if (IsValid(Actor) && IsValid(OwningPawn))
		{
			const float Distance = OwningPawn->GetDistanceTo(Actor);
			if (Distance < ClosetDistance)
			{
				ClosetDistance = Distance;
				ClosetActor = Actor;
			}
		}
	}

	UBTFunctionLibrary::SetBlackboardValueAsFloat(this, DistanceToTargetSelector, ClosetDistance);
	UBTFunctionLibrary::SetBlackboardValueAsObject(this, TargetToFollowSelector, ClosetActor);
}
