// Copyright 2025 Dale "Stropheum" Diaz

#include "CharacterBase.h"
#include "AbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
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
		Server_Kill();
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

UCharacterBaseAttributeSet* ACharacterBase::GetAttributeSet() const
{
	return AttributeSet;
}

void ACharacterBase::PostInitProperties()
{
	Super::PostInitProperties();
	bReplicates = true;
}

void ACharacterBase::Kill()
{
	if (HasAuthority())
	{
		Server_Kill_Implementation();
	}
	else
	{
		Server_Kill();
	}	
}

void ACharacterBase::Server_Kill_Implementation()
{
	Multicast_Ragdoll();
}

void ACharacterBase::Multicast_Ragdoll_Implementation()
{
	if (UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement();
		ensureMsgf(CharacterMovementComponent != nullptr, TEXT("No Character Movement Component found for %s"), *GetName()))
	{
		CharacterMovementComponent->DisableMovement();
	}
	
	if (const auto Capsule = GetCapsuleComponent(); Capsule != nullptr)
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
