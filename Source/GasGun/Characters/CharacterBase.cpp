﻿// Copyright 2025 Dale "Stropheum" Diaz

#include "CharacterBase.h"
#include "AbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "GasGun/AbilitySystem/AttributeSets/CharacterBaseAttributeSet.h"
#include "Net/UnrealNetwork.h"


ACharacterBase::ACharacterBase()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComponent");
	AttributeSet = CreateDefaultSubobject<UCharacterBaseAttributeSet>("AttributeSet");
	GameplayTasksComponent = CreateDefaultSubobject<UGameplayTasksComponent>("GameplayTasksComponent");
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

void ACharacterBase::OnHealthChangeCallback(const FOnAttributeChangeData& OnAttributeChangeData)
{
	if (OnAttributeChangeData.NewValue <= 0.f)
	{
		Kill();
	}
}

void ACharacterBase::OnMaxHealthChangeCallback(const FOnAttributeChangeData& OnAttributeChangeData) {}

void ACharacterBase::OnShieldChangeCallback(const FOnAttributeChangeData& OnAttributeChangeData) {}

void ACharacterBase::OnMaxShieldChangeCallback(const FOnAttributeChangeData& OnAttributeChangeData) {}

void ACharacterBase::OnShieldRegenRateChangeCallback(const FOnAttributeChangeData& OnAttributeChangeData) {}

void ACharacterBase::OnShieldRegenDelayChangeCallback(const FOnAttributeChangeData& OnAttributeChangeData) {}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	check(AbilitySystemComponent);
	check(AttributeSet);

	AbilitySystemComponent->AddAttributeSetSubobject(AttributeSet.Get());
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

UAbilitySystemComponent* ACharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ACharacterBase::Kill()
{
	Ragdoll();
}

void ACharacterBase::Ragdoll()
{
	if (const auto Capsule = GetCapsuleComponent();
		ensureMsgf(Capsule != nullptr, TEXT("No Capsule Component found for %s"), *GetName()))
	{
		Capsule->DestroyComponent();
	}
	if (const auto MeshComponent = GetMesh();
		ensureMsgf(MeshComponent != nullptr, TEXT("No Mesh Component found for %s"), *GetName()))
	{
		MeshComponent->SetSimulatePhysics(true);
	}
}

void ACharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACharacterBase, AbilitySystemComponent);
	DOREPLIFETIME(ACharacterBase, AttributeSet);
	DOREPLIFETIME(ACharacterBase, GameplayTasksComponent);
}
