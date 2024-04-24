// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#define STATE_ATTACK_LIGHT FGameplayTag::RequestGameplayTag(FName("State.Attack.Light"))
#define STATE_ATTACK_LIGHT_NEXT FGameplayTag::RequestGameplayTag(FName("State.Attack.Light.Next"))

#define STATE_DODGE FGameplayTag::RequestGameplayTag(FName("State.Dodge"))
#define STATE_JUMP FGameplayTag::RequestGameplayTag(FName("State.Jump"))

#define STATE_EQUIPMENT FGameplayTag::RequestGameplayTag(FName("State.Equipment"))
#define STATE_EQUIPMENT_ON FGameplayTag::RequestGameplayTag(FName("State.Equipment.On"))

#define STATE_GUARD FGameplayTag::RequestGameplayTag(FName("State.Guard"))
#define STATE_HIT FGameplayTag::RequestGameplayTag(FName("State.Hit"))
#define STATE_DEAD FGameplayTag::RequestGameplayTag(FName("State.Dead"))

#define DATA_DAMAGE FGameplayTag::RequestGameplayTag(FName("Data.Damage"))
#define GAMEPLAYCUE_ATTACKHIT FGameplayTag::RequestGameplayTag(FName("GameplayCue.AttackHit"))