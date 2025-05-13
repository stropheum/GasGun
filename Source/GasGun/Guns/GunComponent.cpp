// Copyright 2025 Dale "Stropheum" Diaz

#include "GunComponent.h"

#include "AbilitySystemComponent.h"
#include "../Characters/PlayerCharacter.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameplayAbilitySpec.h"
#include "Animation/AnimInstance.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#include "GasGun/AbilitySystem/Abilities/FireGunAbility.h"

UGunComponent::UGunComponent()
{
	MuzzleOffset = FVector(56.5f, 14.25f, 11.3f);
}

void UGunComponent::Fire()
{
	if (!CharacterWeakPtr.IsValid() || CharacterWeakPtr->GetController() == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Attempted to fire with improper conditions"));
		return;
	}

	if (!FireAbilityHandle.IsValid() || !FireGunAbility)
	{
		UE_LOG(LogTemp, Warning, TEXT("No FireRifleAbility set"));
		return;
	}

	UAbilitySystemComponent* Asc = CharacterWeakPtr->GetAbilitySystemComponent();
	FGameplayAbilitySpec* AbilitySpec = Asc->FindAbilitySpecFromHandle(FireAbilityHandle);
	ensureMsgf(AbilitySpec, TEXT("Ability spec not found for fire ability handle"));

	Asc->TryActivateAbility(FireAbilityHandle);
}

FVector UGunComponent::GetProjectileSpawnLocation() const
{
	const APlayerController* PlayerController = Cast<APlayerController>(CharacterWeakPtr->GetController());
	const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
	return GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);
}

bool UGunComponent::AttachWeapon(APlayerCharacter* TargetCharacter)
{
	CharacterWeakPtr = TargetCharacter;

	if (CharacterWeakPtr == nullptr || CharacterWeakPtr->GetInstanceComponents().FindItemByClass<UGunComponent>())
	{
		return false;
	}

	const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(CharacterWeakPtr->GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));

	if (FireWeaponAbilityClass && CharacterWeakPtr->GetAbilitySystemComponent())
	{
		const FGameplayAbilitySpec AbilitySpec(FireWeaponAbilityClass, 1, -1, this);
		FireAbilityHandle = CharacterWeakPtr->GetAbilitySystemComponent()->GiveAbility(AbilitySpec);
		FireGunAbility = Cast<UFireGunAbility>(AbilitySpec.Ability);
		check(FireGunAbility);
	}

	if (const APlayerController* PlayerController = Cast<APlayerController>(CharacterWeakPtr->GetController()))
	{
		if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(CharacterWeakPtr))
		{
			PlayerCharacter->SetWeapon(this);
		}

		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(FireMappingContext, 1);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &UGunComponent::Fire);
		}
	}

	return true;
}

void UGunComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (CharacterWeakPtr.IsValid())
	{
		if (const APlayerController* PlayerController = Cast<APlayerController>(CharacterWeakPtr->GetController()))
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				Subsystem->RemoveMappingContext(FireMappingContext);
			}
		}
	}

	Super::EndPlay(EndPlayReason);
}
