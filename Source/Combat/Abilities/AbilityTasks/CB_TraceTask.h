// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "CB_TraceTask.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTraceResultDelegate, const FGameplayAbilityTargetDataHandle&, TargetDataHandle);

class ACB_TraceTargetActor;
/**
 * 
 */
UCLASS()
class COMBAT_API UCB_TraceTask : public UAbilityTask
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "WaitForTrace", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UCB_TraceTask* CreateTask(UGameplayAbility* OwningAbility, TSubclassOf<ACB_TraceTargetActor> TargetActorClass);
	virtual void Activate() override;
	virtual void OnDestroy(bool AbilityEnded) override;

	void SpawnAndInitializeTargetActor();
	void FinalizeTargetActor();

protected:
	void OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& DataHandle);

public:
	UPROPERTY(BlueprintAssignable)
	FTraceResultDelegate OnComplete;

protected:
	UPROPERTY()
	TSubclassOf<ACB_TraceTargetActor> TargetActorClass;

	UPROPERTY()
	TObjectPtr<ACB_TraceTargetActor> SpawnedTargetActor;
};
