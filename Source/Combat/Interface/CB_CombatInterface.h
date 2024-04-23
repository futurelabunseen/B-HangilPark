// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CB_CombatInterface.generated.h"

UINTERFACE(MinimalAPI)
class UCB_CombatInterface : public UInterface
{
	GENERATED_BODY()
};

class COMBAT_API ICB_CombatInterface
{
	GENERATED_BODY()

public:
	virtual FVector GetSocketLocation(const FName SocketName) = 0;
	virtual FVector GetWeaponSocketLocation(const FName SocketName) = 0;
};
