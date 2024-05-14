// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayTagContainer.h"
#include "CB_TrailInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCB_TrailInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class COMBAT_API ICB_TrailInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void TrailStart(FGameplayTag Tag) = 0;
};
