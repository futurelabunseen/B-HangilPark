// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/CB_BaseWeapon.h"
#include "CB_Sword.generated.h"

class UBoxComponent;
/**
 * 
 */
UCLASS()
class COMBAT_API ACB_Sword : public ACB_BaseWeapon
{
	GENERATED_BODY()
public:
	ACB_Sword();

	virtual void Tick(float DeltaTime) override;

	void CollisionOn();
	void CollisionOff();

	void TrailStart();
	void TrailEnd();
//protected:
//	virtual void BeginPlay() override;
//
//	UFUNCTION()
//	void CollisionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
//		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	// 콜리전 지우고 소켓 대상으로 BoxTrace 진행
	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	TObjectPtr<UBoxComponent> BoxCollision;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UParticleSystemComponent> TrailParticle;

	bool bCollisionOn = false;

};
