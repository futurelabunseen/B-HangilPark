// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/CB_BaseWeapon.h"
#include "CB_Sword.generated.h"

class UBoxComponent;
class UNiagaraComponent;

UCLASS()
class COMBAT_API ACB_Sword : public ACB_BaseWeapon
{
	GENERATED_BODY()
public:
	ACB_Sword();

	virtual void Tick(float DeltaTime) override;

	void AttackCheck(bool IsActive);

	void TrailStart();
	void TrailEnd();
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void BoxOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	void BoxTrace(FHitResult& BoxHit);

private:
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UParticleSystemComponent> TrailParticle;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraComponent> TrailEffect;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> BoxCollision;
	
	UPROPERTY()
	TArray<TObjectPtr<AActor>> IgnoreActors;

};
