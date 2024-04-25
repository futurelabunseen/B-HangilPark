// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "CB_BaseWeapon.generated.h"

UCLASS()
class COMBAT_API ACB_BaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	ACB_BaseWeapon();

	void Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator);
	void AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName);
	FORCEINLINE UStaticMeshComponent* GetWeaponMesh() const { return WeaponMesh; }

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> WeaponMesh;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	FGameplayTag WeaponType;
};
