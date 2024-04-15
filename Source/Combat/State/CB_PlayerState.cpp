// Fill out your copyright notice in the Description page of Project Settings.

#include "CB_PlayerState.h"
#include "AbilitySystemComponent.h"
#include "Attribute/CB_CharacterAttributeSet.h"

ACB_PlayerState::ACB_PlayerState()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	AttributeSet = CreateDefaultSubobject<UCB_CharacterAttributeSet>(TEXT("AttributeSet"));
}