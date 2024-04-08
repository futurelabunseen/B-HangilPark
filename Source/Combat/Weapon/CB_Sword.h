// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/CB_BaseWeapon.h"
#include "CB_Sword.generated.h"

class UBoxComponent;
/**
 * 
 */
UCLASS()
class COMBAT_API ACB_Sword : public ACB_BaseWeapon
{
	GENERATED_BODY()
public:
	ACB_Sword();

	virtual void Tick(float DeltaTime) override;

	void TrailStart();
	void TrailEnd();

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UParticleSystemComponent> TrailParticle;

};
