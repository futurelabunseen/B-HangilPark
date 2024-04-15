// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "CB_TestActor.generated.h"

class UAbilitySystemComponent;
class UBoxComponent;
class UStaticMeshComponent;
class UGameplayEffect;

UCLASS()
class COMBAT_API ACB_TestActor : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:	
	ACB_TestActor();

	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override { return ASC; }
	virtual void NotifyActorBeginOverlap(class AActor* Other) override;

protected:
	virtual void PostInitializeComponents() override;

	void ApplyEffectToTarget(AActor* Target);
	void InvokeGameplayCue(AActor* Target);

protected:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> ASC;

	UPROPERTY(VisibleAnywhere, Category = Box)
	TObjectPtr<UBoxComponent> Trigger;

	UPROPERTY(VisibleAnywhere, Category = Box)
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(EditAnywhere, Category = GAS)
	TSubclassOf<UGameplayEffect> GameplayEffectClass;

	UPROPERTY(EditAnywhere, Category = GAS, Meta = (Categories = GameplayCue))
	FGameplayTag GameplayCueTag;
};
