// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GameplayTagContainer.h"
#include "CB_AN_SendEvent.generated.h"

/**
 * 
 */
UCLASS()
class COMBAT_API UCB_AN_SendEvent : public UAnimNotify
{
	GENERATED_BODY()
protected:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:

	UPROPERTY(EditAnywhere, Meta = (Categories = Event))
	FGameplayTagContainer TriggerGameplayTag;

	UPROPERTY(EditAnywhere)
	bool bIsActive;
};
