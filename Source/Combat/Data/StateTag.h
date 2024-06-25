// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#define STATE_ATTACK FGameplayTag::RequestGameplayTag(FName("State.Attack"))
#define STATE_ATTACK_LIGHT FGameplayTag::RequestGameplayTag(FName("State.Attack.Light"))
#define STATE_ATTACK_LIGHT_NEXT FGameplayTag::RequestGameplayTag(FName("State.Attack.Light.Next"))
#define STATE_ATTACK_SKILL_Q FGameplayTag::RequestGameplayTag(FName("State.Attack.Skill.Q"))
#define STATE_ATTACK_SKILL_E FGameplayTag::RequestGameplayTag(FName("State.Attack.Skill.E"))

#define STATE_DODGE FGameplayTag::RequestGameplayTag(FName("State.Dodge"))
#define STATE_JUMP FGameplayTag::RequestGameplayTag(FName("State.Jump"))

#define STATE_EQUIPMENT FGameplayTag::RequestGameplayTag(FName("State.Equipment"))
#define STATE_EQUIPMENT_ON FGameplayTag::RequestGameplayTag(FName("State.Equipment.On"))

#define STATE_GUARD FGameplayTag::RequestGameplayTag(FName("State.Guard"))
#define STATE_HIT FGameplayTag::RequestGameplayTag(FName("State.Hit"))
#define STATE_HIT_LIGHT FGameplayTag::RequestGameplayTag(FName("State.Hit.Light"))
#define STATE_HIT_SKILL FGameplayTag::RequestGameplayTag(FName("State.Hit.SKill"))
#define STATE_HIT_SKILL_HEAVY FGameplayTag::RequestGameplayTag(FName("State.Hit.SKill.Heavy"))
#define STATE_DEAD FGameplayTag::RequestGameplayTag(FName("State.Dead"))
#define STATE_HEAL FGameplayTag::RequestGameplayTag(FName("State.Heal"))
#define STATE_PARRY FGameplayTag::RequestGameplayTag(FName("State.Parry"))

#define DATA_DAMAGE FGameplayTag::RequestGameplayTag(FName("Data.Damage"))

#define GAMEPLAYCUE_ATTACKHIT FGameplayTag::RequestGameplayTag(FName("GameplayCue.AttackHit"))
#define GAMEPLAYCUE_BLOCKHIT FGameplayTag::RequestGameplayTag(FName("GameplayCue.BlockHit"))

#define STATE_DEBUFF_STUN FGameplayTag::RequestGameplayTag(FName("State.Debuff.Stun"))