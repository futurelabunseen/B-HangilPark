// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CB_LockOnComponent.generated.h"

class UUserWidget;
class UWidgetComponent;
class APlayerController;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COMBAT_API UCB_LockOnComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCB_LockOnComponent();
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void TargetActor();

	UFUNCTION(BlueprintCallable)
	void TargetActorWithAxisInput(float AxisValue);

	FORCEINLINE bool IsLocked() const { return bTargetLocked && LockedOnTargetActor; }

protected:
	virtual void BeginPlay() override;

private:
	void TargetLockOff();

	TArray<AActor*> GetAllActorsOfClass(TSubclassOf<AActor> ActorClass) const;
	TArray<AActor*> FindTargetsInRange(TArray<AActor*> ActorsToLook, float RangeMin, float RangeMax) const;

	AActor* FindNearestTarget(TArray<AActor*> Actors) const;

	bool LineTrace(FHitResult& OutHitResult, const AActor* OtherActor, const TArray<AActor*>& ActorsToIgnore) const;
	bool LineTraceForActor(const AActor* OtherActor, const TArray<AActor*>& ActorsToIgnore) const;

	bool ShouldBreakLineOfSight() const;
	void BreakLineOfSight();

	bool IsInViewport(const AActor* TargetActor) const;

	float GetDistanceFromCharacter(const AActor* OtherActor) const { return OwnerActor->GetDistanceTo(OtherActor); }

	FRotator GetControlRotationOnTarget(const AActor* OtherActor) const;
	void SetControlRotationOnTarget(AActor* TargetActor) const;
	void ControlRotation(bool ShouldControlRotation) const;

	float GetAngleUsingCameraRotation(const AActor* ActorToLook) const;
	float GetAngleUsingCharacterRotation(const AActor* ActorToLook) const;

	static FRotator FindLookAtRotation(const FVector Start, const FVector Target) { return FRotationMatrix::MakeFromX(Target - Start).Rotator(); }

	void CreateAndAttachTargetLockedOnWidgetComponent(AActor* TargetActor);

	void TargetLockOn(AActor* TargetToLockOn);
	void ResetIsSwitchingTarget();
	bool ShouldSwitchTargetActor(float AxisValue);

	void SetupLocalPlayerController();

private:
	FORCEINLINE bool GetTargetLockedStatus() const { return bTargetLocked; }
	FORCEINLINE AActor* GetLockedOnTargetActor() const { return LockedOnTargetActor; }

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float MinimumDistanceToEnable = 1200.0f;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> TargetableActors;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TEnumAsByte<ECollisionChannel> TargetableCollisionChannel;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	bool bShouldControlRotation = true;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	bool bIgnoreLookInput = true;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float BreakLineOfSightDelay = 2.0f;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float StartRotatingThreshold = 0.85f;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	bool bShouldDrawLockedOnWidget = true;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> LockedOnWidgetClass;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float LockedOnWidgetDrawSize = 32.0f;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	FName LockedOnWidgetParentSocket = FName("spine_03");

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	FVector LockedOnWidgetRelativeLocation = FVector(0.0f, 0.0f, 0.0f);

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	bool bAdjustPitchBasedOnDistanceToTarget = true;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float PitchDistanceCoefficient = -0.2f;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float PitchDistanceOffset = 60.0f;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float PitchMin = -50.0f;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float PitchMax = -20.0f;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	bool bEnableStickyTarget = false;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float AxisMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float StickyRotationThreshold = 30.0f;

private:
	UPROPERTY()
	AActor* OwnerActor;

	UPROPERTY()
	APawn* OwnerPawn;

	UPROPERTY()
	APlayerController* OwnerPlayerController;

	UPROPERTY()
	UWidgetComponent* TargetLockedOnWidgetComponent;

	UPROPERTY()
	AActor* LockedOnTargetActor;

	FTimerHandle LineOfSightBreakTimerHandle;
	FTimerHandle SwitchingTargetTimerHandle;

	bool bIsBreakingLineOfSight = false;
	bool bIsSwitchingTarget = false;
	bool bTargetLocked = false;
	float ClosestTargetDistance = 0.0f;

	bool bDesireToSwitch = false;
	float StartRotatingStack = 0.0f;
};
