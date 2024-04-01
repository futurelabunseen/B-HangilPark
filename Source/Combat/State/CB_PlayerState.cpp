// Fill out your copyright notice in the Description page of Project Settings.

#include "CB_PlayerState.h"
#include "AbilitySystemComponent.h"

ACB_PlayerState::ACB_PlayerState()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));

}