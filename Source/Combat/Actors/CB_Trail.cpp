// Fill out your copyright notice in the Description page of Project Settings.

#include "Actors/CB_Trail.h"
#include "Components/PoseableMeshComponent.h"
#include "Kismet/KismetMaterialLibrary.h"

ACB_Trail::ACB_Trail()
{
 	PrimaryActorTick.bCanEverTick = true;

	PoseableMesh = CreateDefaultSubobject<UPoseableMeshComponent>(TEXT("PoseableMesh"));
	RootComponent = PoseableMesh;
}


void ACB_Trail::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bSpawned == true)
	{
		if (PoseableMesh && Material)
		{
			fAlphaCount -= DeltaTime;

			for (int i = 0; i < Materials.Num(); ++i)
				Materials[i]->SetScalarParameterValue("Opacity", fAlphaCount / fQuarterAlpha);
			
			if (fAlphaCount < 0)
				Destroy();
		}
	}
}

void ACB_Trail::Init(USkeletalMeshComponent* Pawn)
{
	if (PoseableMesh && Material)
	{
		PoseableMesh->CopyPoseFromSkeletalComponent(Pawn);

		TArray<UMaterialInterface*> Mats = PoseableMesh->GetMaterials();

		for (int i = 0; i < Mats.Num(); ++i)
		{
			Materials.Add(UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), Material));
			PoseableMesh->SetMaterial(i, Materials[i]);
		}

		fQuarterAlpha = fAlphaCount = 0.5f;
		bSpawned = true;
	}
}

