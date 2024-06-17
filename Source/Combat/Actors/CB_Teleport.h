// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CB_Teleport.generated.h"

class UCapsuleComponent;
class UNiagaraComponent;

UCLASS()
class COMBAT_API ACB_Teleport : public AActor
{
	GENERATED_BODY()
	
public:	
	ACB_Teleport();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnCapsuleOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	void SetLevelName(const FString& Name) { LevelName = Name; }
private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	FString LevelName = FString("Defaults");

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> TeleportMesh;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCapsuleComponent> TeleportArea;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraComponent> TeleportEffect;

};
