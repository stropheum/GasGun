// Copyright 2025 Dale "Stropheum" Diaz

#include "CharacterBase.h"
#include "AbilitySystemComponent.h"
#include "GasGun/AbilitySystem/AttributeSets/CharacterBaseAttributeSet.h"


ACharacterBase::ACharacterBase()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComponent");
	AttributeSet = CreateDefaultSubobject<UCharacterBaseAttributeSet>("AttributeSet");
	GameplayTasksComponent = CreateDefaultSubobject<UGameplayTasksComponent>("GameplayTasksComponent");
	PrimaryActorTick.bCanEverTick = true;
}

void ACharacterBase::OnHealthChangeCallback(const FOnAttributeChangeData& OnAttributeChangeData)
{
}

void ACharacterBase::OnMaxHealthChangeCallback(const FOnAttributeChangeData& OnAttributeChangeData)
{
}

void ACharacterBase::OnShieldChangeCallback(const FOnAttributeChangeData& OnAttributeChangeData)
{
}

void ACharacterBase::OnMaxShieldChangeCallback(const FOnAttributeChangeData& OnAttributeChangeData)
{
}

void ACharacterBase::OnShieldRegenRateChangeCallback(const FOnAttributeChangeData& OnAttributeChangeData)
{
}

void ACharacterBase::OnShieldRegenDelayChangeCallback(const FOnAttributeChangeData& OnAttributeChangeData)
{
}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	check(AbilitySystemComponent);
	check(AttributeSet);
	
	AbilitySystemComponent->AddAttributeSetSubobject(AttributeSet);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute())
	                      .AddUObject(this, &ACharacterBase::OnHealthChangeCallback);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMaxHealthAttribute())
	                      .AddUObject(this, &ACharacterBase::OnMaxHealthChangeCallback);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetShieldAttribute())
	                      .AddUObject(this, &ACharacterBase::OnShieldChangeCallback);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMaxShieldAttribute())
	                      .AddUObject(this, &ACharacterBase::OnMaxShieldChangeCallback);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetShieldRegenRateAttribute())
	                      .AddUObject(this, &ACharacterBase::OnShieldRegenRateChangeCallback);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetShieldRegenDelayAttribute())
	                      .AddUObject(this, &ACharacterBase::OnShieldRegenDelayChangeCallback);
}

void ACharacterBase::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
