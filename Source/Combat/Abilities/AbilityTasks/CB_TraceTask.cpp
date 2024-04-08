// Fill out your copyright notice in the Description page of Project Settings.

#include "Abilities/AbilityTasks/CB_TraceTask.h"
#include "Abilities/TargetActor/CB_TraceTargetActor.h"
#include "AbilitySystemComponent.h"

UCB_TraceTask* UCB_TraceTask::CreateTask(UGameplayAbility* OwningAbility, TSubclassOf<ACB_TraceTargetActor> TargetActorClass)
{
	UCB_TraceTask* NewTask = NewAbilityTask<UCB_TraceTask>(OwningAbility);
	NewTask->TargetActorClass = TargetActorClass;
	return NewTask;
}

void UCB_TraceTask::Activate()
{
	Super::Activate();

	SpawnAndInitializeTargetActor();
	FinalizeTargetActor();

	SetWaitingOnAvatar();
}

void UCB_TraceTask::OnDestroy(bool AbilityEnded)
{
	if (SpawnedTargetActor)
		SpawnedTargetActor->Destroy();

	Super::OnDestroy(AbilityEnded);
}

void UCB_TraceTask::SpawnAndInitializeTargetActor()
{
	SpawnedTargetActor = Cast<ACB_TraceTargetActor>(Ability->GetWorld()->SpawnActorDeferred<AGameplayAbilityTargetActor>(TargetActorClass, FTransform::Identity, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn));
	if (SpawnedTargetActor)
	{
		SpawnedTargetActor->SetShowDebug(true);
		SpawnedTargetActor->TargetDataReadyDelegate.AddUObject(this, &UCB_TraceTask::OnTargetDataReadyCallback);
	}
}

void UCB_TraceTask::FinalizeTargetActor()
{
	UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();
	if (ASC)
	{
		const FTransform SpawnTransform = ASC->GetAvatarActor()->GetTransform();
		SpawnedTargetActor->FinishSpawning(SpawnTransform);

		ASC->SpawnedTargetActors.Push(SpawnedTargetActor);
		SpawnedTargetActor->StartTargeting(Ability);
		SpawnedTargetActor->ConfirmTargeting();
	}
}

void UCB_TraceTask::OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& DataHandle)
{
	if (ShouldBroadcastAbilityTaskDelegates())
		OnComplete.Broadcast(DataHandle);

	EndTask();
}
