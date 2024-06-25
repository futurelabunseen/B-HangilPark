// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/CB_BaseWeapon.h"
#include "Data/StateTag.h"
#include "CB_Sword.generated.h"

class UNiagaraComponent;

UCLASS()
class COMBAT_API ACB_Sword : public ACB_BaseWeapon
{
	GENERATED_BODY()
public:
	ACB_Sword();

	void TrailStart();
	void TrailEnd();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UParticleSystemComponent> TrailParticle;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraComponent> TrailEffect;
};
