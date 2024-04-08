// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "CB_ANS_Trail.generated.h"

class USkeletalMeshComponent;
class UAnimSequenceBase;
/**
 * 
 */
UCLASS()
class COMBAT_API UCB_ANS_Trail : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, 
		float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, 
		const FAnimNotifyEventReference& EventReference) override;
};
