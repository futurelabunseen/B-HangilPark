// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon/CB_Sword.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "NiagaraComponent.h"
#include "Kismet/KismetMathLibrary.h"

#include "Kismet/GameplayStatics.h"

ACB_Sword::ACB_Sword()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetupAttachment(GetRootComponent());
	BoxCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BoxCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	
	TrailParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("TrailParticle"));
	TrailParticle->AttachToComponent(GetWeaponMesh(), FAttachmentTransformRules::KeepRelativeTransform);

	TrailEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TrailEffect"));
	TrailEffect->SetupAttachment(GetRootComponent());

}

void ACB_Sword::BeginPlay()
{
	Super::BeginPlay();
	const FVector BotPos = GetWeaponMesh()->GetSocketLocation("StartSocket");
	const FVector TopPos = GetWeaponMesh()->GetSocketLocation("EndSocket");

	TrailEffect->SetFloatParameter("RibbonWidth", (TopPos - BotPos).Length());
	TrailEffect->SetWorldRotation(UKismetMathLibrary::MakeRotFromZ(BotPos - TopPos));
	TrailEffect->SetWorldLocation((TopPos + BotPos) / 2);

	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ACB_Sword::BoxOverlap);

}

void ACB_Sword::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACB_Sword::AttackCheck(bool IsActive)
{
	if (IsActive) 
	{
		BoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		TrailStart();
	}
	else 
	{
		BoxCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		
		TrailEnd();
	}
}

void ACB_Sword::TrailStart()
{
	TrailParticle->BeginTrails("StartSocket", "EndSocket", ETrailWidthMode_FromCentre, 1.f);
	TrailEffect->Activate();
}

void ACB_Sword::TrailEnd()
{
	TrailParticle->EndTrails();
	TrailEffect->Deactivate();
}

void ACB_Sword::BoxOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IgnoreActors.Empty();
	FHitResult BoxHit;
	BoxTrace(BoxHit);

	if (BoxHit.GetActor())
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *BoxHit.GetActor()->GetName());
		// 태그로 어빌리티 호출
	
	}

}

void ACB_Sword::BoxTrace(FHitResult& BoxHit)
{	
	ETraceTypeQuery MyTraceType = UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1);
	
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	ActorsToIgnore.Add(GetOwner());

	for (AActor* Actor : IgnoreActors)
		ActorsToIgnore.AddUnique(Actor);

	const FVector Start = GetWeaponMesh()->GetSocketLocation("StartSocket");
	const FVector End = GetWeaponMesh()->GetSocketLocation("EndSocket");

	//ForDuration, None
	UKismetSystemLibrary::BoxTraceSingle(this, Start, End, FVector(10.f),
		Start.Rotation(), MyTraceType, false, ActorsToIgnore, EDrawDebugTrace::None, BoxHit, true);

	IgnoreActors.AddUnique(BoxHit.GetActor());
}
