// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CB_TrailComponent.generated.h"

namespace TimerRate
{
	const float AutomaticTrailRate = 0.05f;
	const float SprintRate = 0.025f;
};

class ACB_BaseCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COMBAT_API UCB_TrailComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCB_TrailComponent();

	void StartTrail(FGameplayTag Tag);

	UFUNCTION()
	void TrailTimerReset(FGameplayTag Tag);

	void MakeTrail();

	void SprintTrail();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TObjectPtr<ACB_BaseCharacter> Character;

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	TSubclassOf<AActor> ActorToSpawn;

	bool bTrail = true;
};
