// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "GameplayTagContainer.h"
#include "CB_ANS_Branch.generated.h"

UCLASS()
class COMBAT_API UCB_ANS_Branch : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, 
		float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
};
