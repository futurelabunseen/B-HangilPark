// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"	
#include "CB_ANS_Activate.generated.h"

class USkeletalMeshComponent;
class UAnimSequenceBase;
class ACB_BaseCharacter;
class ACB_Sword;

UCLASS()
class COMBAT_API UCB_ANS_Activate : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, 
		float TotalDuration, const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
		float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, 
		const FAnimNotifyEventReference& EventReference) override;

private:
	UPROPERTY()
	TMap<FString, FHitResult> HitActors;
	// 마지막 ImpactPoint를 저장
	// 1. TMultiMap
	// 2. TArray<TPair<FString, FHitResult>>

	UPROPERTY()
	TObjectPtr<ACB_BaseCharacter> Character;
	
	UPROPERTY()
	TObjectPtr<ACB_Sword> Sword;

};
