// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CB_AN_FootStep.generated.h"

/**
 * 
 */
UCLASS()
class COMBAT_API UCB_AN_FootStep : public UAnimNotify
{
	GENERATED_BODY()
protected:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
		const FAnimNotifyEventReference& EventReference) override;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	FName SocketName;
};
