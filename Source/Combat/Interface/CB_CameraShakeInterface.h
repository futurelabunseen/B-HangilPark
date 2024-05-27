// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CB_CameraShakeInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCB_CameraShakeInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class COMBAT_API ICB_CameraShakeInterface
{
	GENERATED_BODY()

public:
	virtual void DoCameraShake() = 0;
};
