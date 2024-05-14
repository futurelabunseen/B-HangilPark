// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CB_Trail.generated.h"

class UPoseableMeshComponent;
class UMaterialInstance;
class UMaterialInstanceDynamic;
class USkeletalMeshComponent;
class UMaterial;

UCLASS()
class COMBAT_API ACB_Trail : public AActor
{
	GENERATED_BODY()
	
public:	
	ACB_Trail();

	virtual void Tick(float DeltaTime) override;

	void Init(USkeletalMeshComponent* Pawn);

private:
	UPROPERTY(EditAnywhere, Category = "Comp", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPoseableMeshComponent> PoseableMesh;

	UPROPERTY(EditAnywhere, Category = "Comp", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMaterialInstance> Material;

	UPROPERTY(VisibleAnywhere, Category = "Comp", meta = (AllowPrivateAccess = "true"))
	TArray<UMaterialInstanceDynamic*> Materials;

	float	fAlphaCount;
	float	fQuarterAlpha;
	bool	bSpawned = false;

};
