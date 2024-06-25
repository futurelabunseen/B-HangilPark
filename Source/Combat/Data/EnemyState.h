// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	EES_Idle			UMETA(DisplayName = "Idle"),
	EES_Attack			UMETA(DisplayName = "Attack"),
	EES_Guard			UMETA(DisplayName = "Guard"),
	EES_Dodge			UMETA(DisplayName = "Dodge")
};