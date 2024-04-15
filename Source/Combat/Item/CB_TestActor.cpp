// Fill out your copyright notice in the Description page of Project Settings.

#include "Item/CB_TestActor.h"
#include "AbilitySystemComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

ACB_TestActor::ACB_TestActor()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Trigger);
	Mesh->SetupAttachment(Trigger);

	Trigger->SetBoxExtent(FVector(40.0f, 42.0f, 30.0f));
	
	Mesh->SetRelativeLocation(FVector(0.0f, -3.5f, -30.0f));
	Mesh->SetCollisionProfileName(TEXT("NoCollision"));
}

void ACB_TestActor::NotifyActorBeginOverlap(AActor* Other)
{
	Super::NotifyActorBeginOverlap(Other);

	InvokeGameplayCue(Other);
	ApplyEffectToTarget(Other);

	Mesh->SetHiddenInGame(true);
	SetActorEnableCollision(false);
	SetLifeSpan(2.0f);
}

void ACB_TestActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	ASC->InitAbilityActorInfo(this, this);
}

void ACB_TestActor::ApplyEffectToTarget(AActor* Target)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
	if (TargetASC)
	{
		FGameplayEffectContextHandle EffectContext = TargetASC->MakeEffectContext();
		EffectContext.AddSourceObject(this);
		FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, 1, EffectContext);

		if (EffectSpecHandle.IsValid())
			TargetASC->BP_ApplyGameplayEffectSpecToSelf(EffectSpecHandle);
	}
}

void ACB_TestActor::InvokeGameplayCue(AActor* Target)
{
	FGameplayCueParameters Param;
	Param.SourceObject = this;
	Param.Instigator = Target;
	Param.Location = GetActorLocation();
	ASC->ExecuteGameplayCue(GameplayCueTag, Param);
}
