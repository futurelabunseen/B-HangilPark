// Fill out your copyright notice in the Description page of Project Settings.

#include "CB_WaitLandingTask.h"
#include "GameFramework/Character.h"

UCB_WaitLandingTask* UCB_WaitLandingTask::CreateTask(UGameplayAbility* OwningAbility)
{
	UCB_WaitLandingTask* NewTask = NewAbilityTask<UCB_WaitLandingTask>(OwningAbility);
	return NewTask;
}

void UCB_WaitLandingTask::Activate()
{
	Super::Activate();

	ACharacter* Character = CastChecked<ACharacter>(GetAvatarActor());
	Character->LandedDelegate.AddDynamic(this, &UCB_WaitLandingTask::OnLandedCallback);
	Character->Jump();

	SetWaitingOnAvatar();
}

void UCB_WaitLandingTask::OnDestroy(bool AbilityEnded)
{
	ACharacter* Character = CastChecked<ACharacter>(GetAvatarActor());
	Character->LandedDelegate.RemoveDynamic(this, &UCB_WaitLandingTask::OnLandedCallback);

	Super::OnDestroy(AbilityEnded);
}

void UCB_WaitLandingTask::OnLandedCallback(const FHitResult& Hit)
{
	if (ShouldBroadcastAbilityTaskDelegates())
		OnComplete.Broadcast();
}
