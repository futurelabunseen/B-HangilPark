// Fill out your copyright notice in the Description page of Project Settings.

#include "CB_EnemyCharacter.h"
#include "AbilitySystemComponent.h"
#include "Attribute/CB_CharacterAttributeSet.h"
#include "Components/WidgetComponent.h"
#include "UI/Widgets/CB_UserWidget.h"

ACB_EnemyCharacter::ACB_EnemyCharacter()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	AttributeSet = CreateDefaultSubobject<UCB_CharacterAttributeSet>(TEXT("AttributeSet"));

	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());

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

}

void ACB_EnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (UCB_UserWidget* UI = Cast<UCB_UserWidget>(HealthBar->GetUserWidgetObject()))
		UI->SetWidgetController(this);

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
}
