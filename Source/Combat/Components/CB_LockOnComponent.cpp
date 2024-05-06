// Fill out your copyright notice in the Description page of Project Settings.

#include "CB_LockOnComponent.h"
#include "EngineUtils.h"
#include "TimerManager.h"
#include "Camera/CameraComponent.h"
#include "Components/WidgetComponent.h"
#include "Engine/GameViewportClient.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "Character/CB_EnemyCharacter.h"

UCB_LockOnComponent::UCB_LockOnComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	TargetableActors = APawn::StaticClass();
	TargetableCollisionChannel = ECollisionChannel::ECC_Pawn;
}

void UCB_LockOnComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerActor = GetOwner();
	OwnerPawn = Cast<APawn>(OwnerActor);
	SetupLocalPlayerController();
}

void UCB_LockOnComponent::TargetLockOff()
{
	SetupLocalPlayerController();

	ACB_EnemyCharacter* Target = Cast<ACB_EnemyCharacter>(LockedOnTargetActor);
	if (Target)
		Target->SetOutLine(false);
	
	bTargetLocked = false;
	if (TargetLockedOnWidgetComponent)
		TargetLockedOnWidgetComponent->DestroyComponent();

	if (LockedOnTargetActor)
	{
		if (bShouldControlRotation)
			ControlRotation(false);

		if (IsValid(OwnerPlayerController))
			OwnerPlayerController->ResetIgnoreLookInput();
	}

	LockedOnTargetActor = nullptr;
}

TArray<AActor*> UCB_LockOnComponent::GetAllActorsOfClass(TSubclassOf<AActor> ActorClass) const
{
	TArray<AActor*> Actors;
	for (TActorIterator<AActor> ActorIterator(GetWorld(), ActorClass); ActorIterator; ++ActorIterator)
	{
		AActor* Actor = *ActorIterator;
		if (Actor)
			Actors.Add(Actor);
	}

	return Actors;
}

TArray<AActor*> UCB_LockOnComponent::FindTargetsInRange(TArray<AActor*> ActorsToLook, float RangeMin, float RangeMax) const
{
	TArray<AActor*> ActorsInRange;

	for (AActor* Actor : ActorsToLook)
	{
		const float Angle = GetAngleUsingCameraRotation(Actor);
		if (Angle > RangeMin && Angle < RangeMax)
			ActorsInRange.Add(Actor);
	}

	return ActorsInRange;
}

AActor* UCB_LockOnComponent::FindNearestTarget(TArray<AActor*> Actors) const
{
	TArray<AActor*> ActorsHit;

	for (AActor* Actor : Actors)
	{
		TArray<AActor*> ActorsToIgnore;
		const bool bHit = LineTraceForActor(Actor, ActorsToIgnore);
		if (bHit && IsInViewport(Actor))
			ActorsHit.Add(Actor);
	}

	if (ActorsHit.Num() == 0)
		return nullptr;

	float ClosestDistance = ClosestTargetDistance;
	AActor* Target = nullptr;
	for (AActor* Actor : ActorsHit)
	{
		const float Distance = GetDistanceFromCharacter(Actor);
		if (Distance < ClosestDistance)
		{
			ClosestDistance = Distance;
			Target = Actor;
		}
	}

	return Target;
}

bool UCB_LockOnComponent::LineTrace(FHitResult& OutHitResult, const AActor* OtherActor, const TArray<AActor*>& ActorsToIgnore) const
{
	if (!IsValid(OwnerActor) || !IsValid(OtherActor))
		return false;

	TArray<AActor*> IgnoredActors;
	IgnoredActors.Reserve(ActorsToIgnore.Num() + 1);
	IgnoredActors.Add(OwnerActor);
	IgnoredActors.Append(ActorsToIgnore);

	FCollisionQueryParams Params = FCollisionQueryParams(FName("LineTraceSingle"));
	Params.AddIgnoredActors(IgnoredActors);

	if (const UWorld* World = GetWorld(); IsValid(World))
	{
		return World->LineTraceSingleByChannel(OutHitResult, OwnerActor->GetActorLocation(),
			OtherActor->GetActorLocation(), TargetableCollisionChannel, Params);
	}

	return false;
}

bool UCB_LockOnComponent::LineTraceForActor(const AActor* OtherActor, const TArray<AActor*>& ActorsToIgnore) const
{
	FHitResult HitResult;
	const bool bHit = LineTrace(HitResult, OtherActor, ActorsToIgnore);
	if (bHit)
	{
		const AActor* HitActor = HitResult.GetActor();
		if (HitActor == OtherActor)
			return true;
	}

	return false;
}

bool UCB_LockOnComponent::ShouldBreakLineOfSight() const
{
	if (!LockedOnTargetActor)
		return true;

	TArray<AActor*> ActorsToIgnore = GetAllActorsOfClass(TargetableActors);
	ActorsToIgnore.Remove(LockedOnTargetActor);

	FHitResult HitResult;
	const bool bHit = LineTrace(HitResult, LockedOnTargetActor, ActorsToIgnore);

	if (bHit && HitResult.GetActor() != LockedOnTargetActor)
		return true;

	return false;
}

void UCB_LockOnComponent::BreakLineOfSight()
{
	bIsBreakingLineOfSight = false;
	if (ShouldBreakLineOfSight())
		TargetLockOff();
}

bool UCB_LockOnComponent::IsInViewport(const AActor* TargetActor) const
{
	if (!IsValid(OwnerPlayerController))
		return true;

	FVector2D ScreenLocation;
	OwnerPlayerController->ProjectWorldLocationToScreen(TargetActor->GetActorLocation(), ScreenLocation);

	FVector2D ViewportSize;
	GetWorld()->GetGameViewport()->GetViewportSize(ViewportSize);

	return ScreenLocation.X > 0 && ScreenLocation.Y > 0 && ScreenLocation.X < ViewportSize.X && ScreenLocation.Y < ViewportSize.Y;

}

void UCB_LockOnComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bTargetLocked || !LockedOnTargetActor)
		return;

	SetControlRotationOnTarget(LockedOnTargetActor);

	if (GetDistanceFromCharacter(LockedOnTargetActor) > MinimumDistanceToEnable)
		TargetLockOff();

	if (ShouldBreakLineOfSight() && !bIsBreakingLineOfSight)
	{
		if (BreakLineOfSightDelay <= 0)
			TargetLockOff();

		else
		{
			bIsBreakingLineOfSight = true;
			GetWorld()->GetTimerManager().SetTimer(LineOfSightBreakTimerHandle, this,
				&UCB_LockOnComponent::BreakLineOfSight, BreakLineOfSightDelay);
		}
	}
}

void UCB_LockOnComponent::TargetActor()
{
	ClosestTargetDistance = MinimumDistanceToEnable;

	if (bTargetLocked)
		TargetLockOff();
	else
	{
		const TArray<AActor*> Actors = GetAllActorsOfClass(TargetableActors);
		LockedOnTargetActor = FindNearestTarget(Actors);
		TargetLockOn(LockedOnTargetActor);
	}
}

void UCB_LockOnComponent::TargetActorWithAxisInput(float AxisValue)
{
	if (!bTargetLocked || !LockedOnTargetActor || !ShouldSwitchTargetActor(AxisValue) || bIsSwitchingTarget)
		return;

	AActor* CurrentTarget = LockedOnTargetActor;

	const float RangeMin = AxisValue < 0 ? 0 : 180;
	const float RangeMax = AxisValue < 0 ? 180 : 360;

	ClosestTargetDistance = MinimumDistanceToEnable;

	TArray<AActor*> Actors = GetAllActorsOfClass(TargetableActors);
	TArray<AActor*> ActorsToLook;
	TArray<AActor*> ActorsToIgnore;

	ActorsToIgnore.Add(CurrentTarget);

	for (AActor* Actor : Actors)
	{
		const bool bHit = LineTraceForActor(Actor, ActorsToIgnore);
		if (bHit && IsInViewport(Actor))
			ActorsToLook.Add(Actor);

	}

	TArray<AActor*> TargetsInRange = FindTargetsInRange(ActorsToLook, RangeMin, RangeMax);

	AActor* ActorToTarget = nullptr;
	for (AActor* Actor : TargetsInRange)
	{
		const float Distance = GetDistanceFromCharacter(Actor);
		if (Distance < MinimumDistanceToEnable)
		{
			const float RelativeActorsDistance = CurrentTarget->GetDistanceTo(Actor);
			if (RelativeActorsDistance < ClosestTargetDistance)
			{
				ClosestTargetDistance = RelativeActorsDistance;
				ActorToTarget = Actor;
			}
		}
	}

	if (ActorToTarget)
	{
		if (SwitchingTargetTimerHandle.IsValid())
			SwitchingTargetTimerHandle.Invalidate();

		TargetLockOff();
		LockedOnTargetActor = ActorToTarget;
		TargetLockOn(ActorToTarget);

		GetWorld()->GetTimerManager().SetTimer(SwitchingTargetTimerHandle, this,
			&UCB_LockOnComponent::ResetIsSwitchingTarget, bIsSwitchingTarget ? 0.25f : 0.5f);

		bIsSwitchingTarget = true;
	}
}

FRotator UCB_LockOnComponent::GetControlRotationOnTarget(const AActor* OtherActor) const
{
	if (!IsValid(OwnerPlayerController))
		return FRotator::ZeroRotator;

	const FRotator ControlRotation = OwnerPlayerController->GetControlRotation();

	const FVector CharacterLocation = OwnerActor->GetActorLocation();
	const FVector OtherActorLocation = OtherActor->GetActorLocation();

	const FRotator LookRotation = FRotationMatrix::MakeFromX(OtherActorLocation - CharacterLocation).Rotator();
	float Pitch = LookRotation.Pitch;
	FRotator TargetRotation;
	if (bAdjustPitchBasedOnDistanceToTarget)
	{
		const float DistanceToTarget = GetDistanceFromCharacter(OtherActor);
		const float PitchInRange = (DistanceToTarget * PitchDistanceCoefficient + PitchDistanceOffset) * -1.0f;
		const float PitchOffset = FMath::Clamp(PitchInRange, PitchMin, PitchMax);

		Pitch = Pitch + PitchOffset;
		TargetRotation = FRotator(Pitch, LookRotation.Yaw, ControlRotation.Roll);
	}
	else
	{
		if (bIgnoreLookInput)
			TargetRotation = FRotator(Pitch, LookRotation.Yaw, ControlRotation.Roll);
		else
			TargetRotation = FRotator(ControlRotation.Pitch, LookRotation.Yaw, ControlRotation.Roll);
	}

	return FMath::RInterpTo(ControlRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), 9.0f);
}

void UCB_LockOnComponent::SetControlRotationOnTarget(AActor* TargetActor) const
{
	if (!IsValid(OwnerPlayerController))
		return;

	const FRotator ControlRotation = GetControlRotationOnTarget(TargetActor);

	OwnerPlayerController->SetControlRotation(ControlRotation);
}

void UCB_LockOnComponent::ControlRotation(bool ShouldControlRotation) const
{
	if (!IsValid(OwnerPawn))
		return;

	OwnerPawn->bUseControllerRotationYaw = ShouldControlRotation;

	UCharacterMovementComponent* CharacterMovementComponent = OwnerPawn->FindComponentByClass<UCharacterMovementComponent>();
	if (CharacterMovementComponent)
		CharacterMovementComponent->bOrientRotationToMovement = !ShouldControlRotation;
}

float UCB_LockOnComponent::GetAngleUsingCameraRotation(const AActor* ActorToLook) const
{
	UCameraComponent* CameraComponent = OwnerActor->FindComponentByClass<UCameraComponent>();

	if (!CameraComponent)
		return GetAngleUsingCharacterRotation(ActorToLook);

	const FRotator CameraWorldRotation = CameraComponent->GetComponentRotation();
	const FRotator LookAtRotation = FindLookAtRotation(CameraComponent->GetComponentLocation(), ActorToLook->GetActorLocation());

	float YawAngle = CameraWorldRotation.Yaw - LookAtRotation.Yaw;
	if (YawAngle < 0)
		YawAngle = YawAngle + 360;

	return YawAngle;
}

float UCB_LockOnComponent::GetAngleUsingCharacterRotation(const AActor* ActorToLook) const
{
	const FRotator CharacterRotation = OwnerActor->GetActorRotation();
	const FRotator LookAtRotation = FindLookAtRotation(OwnerActor->GetActorLocation(), ActorToLook->GetActorLocation());

	float YawAngle = CharacterRotation.Yaw - LookAtRotation.Yaw;
	if (YawAngle < 0)
		YawAngle = YawAngle + 360;

	return YawAngle;
}

void UCB_LockOnComponent::CreateAndAttachTargetLockedOnWidgetComponent(AActor* TargetActor)
{
	if (!LockedOnWidgetClass)
		return;

	TargetLockedOnWidgetComponent = NewObject<UWidgetComponent>(TargetActor, MakeUniqueObjectName(TargetActor, UWidgetComponent::StaticClass(), FName("TargetLockOn")));
	TargetLockedOnWidgetComponent->SetWidgetClass(LockedOnWidgetClass);

	UMeshComponent* MeshComponent = TargetActor->FindComponentByClass<UMeshComponent>();
	USceneComponent* ParentComponent = MeshComponent && LockedOnWidgetParentSocket != NAME_None ? MeshComponent : TargetActor->GetRootComponent();

	if (IsValid(OwnerPlayerController))
		TargetLockedOnWidgetComponent->SetOwnerPlayer(OwnerPlayerController->GetLocalPlayer());

	TargetLockedOnWidgetComponent->ComponentTags.Add(FName("TargetSystem.LockOnWidget"));
	TargetLockedOnWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	TargetLockedOnWidgetComponent->SetupAttachment(ParentComponent, LockedOnWidgetParentSocket);
	TargetLockedOnWidgetComponent->SetRelativeLocation(LockedOnWidgetRelativeLocation);
	TargetLockedOnWidgetComponent->SetDrawSize(FVector2D(LockedOnWidgetDrawSize, LockedOnWidgetDrawSize));
	TargetLockedOnWidgetComponent->SetVisibility(true);
	TargetLockedOnWidgetComponent->RegisterComponent();
}

void UCB_LockOnComponent::TargetLockOn(AActor* TargetToLockOn)
{
	if (!IsValid(TargetToLockOn))
		return;

	SetupLocalPlayerController();

	bTargetLocked = true;
	if (bShouldDrawLockedOnWidget)
		CreateAndAttachTargetLockedOnWidgetComponent(TargetToLockOn);

	if (bShouldControlRotation)
		ControlRotation(true);

	if (bAdjustPitchBasedOnDistanceToTarget || bIgnoreLookInput)
	{
		if (IsValid(OwnerPlayerController))
			OwnerPlayerController->SetIgnoreLookInput(true);
	}

	ACB_EnemyCharacter* Target = Cast<ACB_EnemyCharacter>(LockedOnTargetActor);
	if (Target)
		Target->SetOutLine(true);
	
}

void UCB_LockOnComponent::ResetIsSwitchingTarget()
{
	bIsSwitchingTarget = false;
	bDesireToSwitch = false;
}

bool UCB_LockOnComponent::ShouldSwitchTargetActor(float AxisValue)
{
	if (bEnableStickyTarget)
	{
		StartRotatingStack += (AxisValue != 0) 
			? AxisValue * AxisMultiplier
			: (StartRotatingStack > 0 ? -AxisMultiplier : AxisMultiplier);

		if (AxisValue == 0 && FMath::Abs(StartRotatingStack) <= AxisMultiplier)
			StartRotatingStack = 0.0f;

		if (FMath::Abs(StartRotatingStack) < StickyRotationThreshold)
		{
			bDesireToSwitch = false;
			return false;
		}

		if (StartRotatingStack * AxisValue > 0)
			StartRotatingStack = (StartRotatingStack > 0) 
				? StickyRotationThreshold 
				: -StickyRotationThreshold;

		else if (StartRotatingStack* AxisValue < 0)
			StartRotatingStack = StartRotatingStack * -1.0f;

		bDesireToSwitch = true;
		return true;
	}

	return FMath::Abs(AxisValue) > StartRotatingThreshold;
}

void UCB_LockOnComponent::SetupLocalPlayerController()
{
	if (!IsValid(OwnerPawn))
		return;

	OwnerPlayerController = Cast<APlayerController>(OwnerPawn->GetController());
}

