// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "CB_ANS_WeaponTrail.generated.h"

class ACB_BaseCharacter;
class ACB_Sword;

/**
 * 
 */
UCLASS()
class COMBAT_API UCB_ANS_WeaponTrail : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
		float TotalDuration, const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
		const FAnimNotifyEventReference& EventReference) override;

private:
	UPROPERTY()
	TObjectPtr<ACB_BaseCharacter> Character;

	UPROPERTY()
	TObjectPtr<ACB_Sword> Sword;
};
