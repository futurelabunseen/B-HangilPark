// Fill out your copyright notice in the Description page of Project Settings.


#include "CB_EnemyCharacter.h"

ACB_EnemyCharacter::ACB_EnemyCharacter()
{
}

void ACB_EnemyCharacter::SetOutLine(bool bIsShow)
{
	GetMesh()->SetRenderCustomDepth(bIsShow);
}
