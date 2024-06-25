// Fill out your copyright notice in the Description page of Project Settings.

#include "CB_EnemyCharacter.h"
#include "AbilitySystemComponent.h"
#include "Attribute/CB_CharacterAttributeSet.h"
#include "AI/CB_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "UI/Widgets/CB_UserWidget.h"
#include "UI/Controller/CB_OverlayWidgetController.h"
#include "MotionWarpingComponent.h"
#include "GameInstance/CB_GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Weapon/CB_BaseWeapon.h"

ACB_EnemyCharacter::ACB_EnemyCharacter()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	AttributeSet = CreateDefaultSubobject<UCB_CharacterAttributeSet>(TEXT("AttributeSet"));
}

void ACB_EnemyCharacter::SetOutLine(bool bIsShow)
{
	GetMesh()->SetRenderCustomDepth(bIsShow);
}

void ACB_EnemyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	ASC->InitAbilityActorInfo(this, this);

	FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(InitStatEffect, Level, EffectContextHandle);
	if (EffectSpecHandle.IsValid())
		ASC->BP_ApplyGameplayEffectSpecToSelf(EffectSpecHandle);

	for (const auto& Ability : Abilities)
	{
		FGameplayAbilitySpec Spec(Ability);
		ASC->GiveAbility(Spec);
	}

	FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	for (TSubclassOf<UGameplayEffect> GameplayEffect : Effects)
	{
		FGameplayEffectSpecHandle NewHandle = ASC->MakeOutgoingSpec(GameplayEffect, 1.f, EffectContext);
		if (NewHandle.IsValid())
			FActiveGameplayEffectHandle ActiveGEHandle = ASC->ApplyGameplayEffectSpecToTarget(
				*NewHandle.Data.Get(), ASC.Get());
	}

	AIController = Cast<ACB_AIController>(NewController);
	if (IsValid(AIController))
	{
		AIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
		AIController->GetBlackboardComponent()->SetValueAsEnum(FName("EnemyState"), (uint8)EEnemyState::EES_Idle);
	}
}

void ACB_EnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	BossOverlay = CreateWidget<UCB_UserWidget>(GetWorld(), BossOverlayClass);
	BossOverlay->SetWidgetController(this);

	if (ASC->DefaultStartingData.Num() > 0)
	{
		UDataTable* InitData = ASC->DefaultStartingData[0].DefaultStartingTable;
		if (IsValid(InitData))
			AttributeSet->InitFromMetaDataTable(InitData);
	}

	if (const UCB_CharacterAttributeSet* AS = Cast<UCB_CharacterAttributeSet>(AttributeSet))
	{
		ASC->GetGameplayAttributeValueChangeDelegate(AS->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data) {
				OnHealthChanged.Broadcast(Data.NewValue);
			}
		);
		ASC->GetGameplayAttributeValueChangeDelegate(AS->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data) {
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);

		OnHealthChanged.Broadcast(AS->GetHealth());
		OnMaxHealthChanged.Broadcast(AS->GetMaxHealth());
	}
	
	FGameplayTagContainer Container;
	Container.AddTag(STATE_EQUIPMENT);
	ASC->TryActivateAbilitiesByTag(Container);
	
}

void ACB_EnemyCharacter::Dead()
{
	const FString& MapName = UGameplayStatics::GetCurrentLevelName(GetWorld());

	BossOverlay->RemoveFromParent();

	if (bIsBoss)
	{
		UCB_GameInstance* GameInstance = Cast<UCB_GameInstance>(GetWorld()->GetGameInstance());
		GameInstance->IncWinCnt();
		GameInstance->ClearThisLevel(MapName);
	}

	if (IsValid(AIController))
		AIController->StopAI();

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]() {
		if (GetWeapon())
			GetWeapon()->Destroy();

		Destroy();

		FVector Location = GetActorLocation();
		Location.Z -= 115.f;
		GetWorld()->SpawnActor<AActor>(Teleport, Location, FRotator(), FActorSpawnParameters());
	}), 2.f, false);
}

void ACB_EnemyCharacter::SetWarpTarget()
{
	Super::SetWarpTarget();
	if (IsValid(TargetActor))
		MotionWarpingComponent->AddOrUpdateWarpTargetFromTransform(TEXT("Target"), TargetActor->GetActorTransform());
}

void ACB_EnemyCharacter::ActivateEnemy()
{
	if (IsValid(AIController))
	{
		AIController->RunBehaviorTree(BehaviorTree);
	}
	if (bIsBoss)
	{
		ACB_BaseCharacter* Player = Cast<ACB_BaseCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		SetTargetActor(Player);
		BossOverlay->AddToViewport();
	}
}
