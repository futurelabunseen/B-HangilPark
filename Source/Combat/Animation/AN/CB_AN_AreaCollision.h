// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GameplayTagContainer.h"
#include "CB_AN_AreaCollision.generated.h"

class ACB_BaseCharacter;
/**
 * 
 */
UCLASS()
class COMBAT_API UCB_AN_AreaCollision : public UAnimNotify
{
	GENERATED_BODY()

protected:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
		const FAnimNotifyEventReference& EventReference) override;

private:
	void DoDamage(TArray<AActor*> HitResults, TArray<AActor*> ActorsToIgnore);
	float CheckTheta(AActor* HitActor, FVector& Vector);

private:
	UPROPERTY(EditAnywhere)
	float SkillRadius = 450.f;

	UPROPERTY(EditAnywhere)
	FGameplayTag GameplayTag;

	UPROPERTY()
	TObjectPtr<ACB_BaseCharacter> Character;
};
