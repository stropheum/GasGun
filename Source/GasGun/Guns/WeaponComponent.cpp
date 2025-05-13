// Copyright 2025 Dale "Stropheum" Diaz

#include "WeaponComponent.h"

#include "AbilitySystemComponent.h"
#include "../Characters/PlayerCharacter.h"
#include "Projectile.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameplayAbilitySpec.h"
#include "Animation/AnimInstance.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#include "GasGun/AbilitySystem/Abilities/FireWeaponAbility.h"

UWeaponComponent::UWeaponComponent()
{
	MuzzleOffset = FVector(56.5f, 14.25f, 11.3f);
}


void UWeaponComponent::Fire()
{
	if (!CharacterWeakPtr.IsValid() || CharacterWeakPtr->GetController() == nullptr)
	{
		return;
	}

	if (ProjectileClass != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			APlayerController* PlayerController = Cast<APlayerController>(CharacterWeakPtr->GetController());
			const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
			const FVector SpawnLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);

			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
			World->SpawnActor<AProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
		}
	}

	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, CharacterWeakPtr->GetActorLocation());
	}

	if (FireAnimation != nullptr)
	{
		UAnimInstance* AnimInstance = CharacterWeakPtr->GetMesh1P()->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}

FVector UWeaponComponent::GetProjectileSpawnLocation() const
{
	const APlayerController* PlayerController = Cast<APlayerController>(CharacterWeakPtr->GetController());
	const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
	return GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);
}

bool UWeaponComponent::AttachWeapon(APlayerCharacter* TargetCharacter)
{
	CharacterWeakPtr = TargetCharacter;

	if (CharacterWeakPtr == nullptr || CharacterWeakPtr->GetInstanceComponents().FindItemByClass<UWeaponComponent>())
	{
		return false;
	}

	const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(CharacterWeakPtr->GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));

	if (FireWeaponAbilityClass && CharacterWeakPtr->GetAbilitySystemComponent())
	{
		const FGameplayAbilitySpec AbilitySpec(FireWeaponAbilityClass, 1, -1, this);
		FireAbilityHandle = CharacterWeakPtr->GetAbilitySystemComponent()->GiveAbility(AbilitySpec);
		FireWeaponAbility = Cast<UFireWeaponAbility>(AbilitySpec.Ability);
		check(FireWeaponAbility);
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
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &UWeaponComponent::Fire);
		}
	}

	return true;
}

void UWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
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
