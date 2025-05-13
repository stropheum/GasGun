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

// Sets default values for this component's properties
UWeaponComponent::UWeaponComponent()
{
	// Default offset from the character location for projectiles to spawn
	MuzzleOffset = FVector(56.5f, 14.25f, 11.3f);
}


void UWeaponComponent::Fire()
{
	if (!CharacterWeakPtr.IsValid() || CharacterWeakPtr->GetController() == nullptr)
	{
		return;
	}

	// Try and fire a projectile
	if (ProjectileClass != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			APlayerController* PlayerController = Cast<APlayerController>(CharacterWeakPtr->GetController());
			const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
			const FVector SpawnLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);
	
			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	
			// Spawn the projectile at the muzzle
			World->SpawnActor<AProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
		}
	}
	
	// Try and play the sound if specified
	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, CharacterWeakPtr->GetActorLocation());
	}
	
	// Try and play a firing animation if specified
	if (FireAnimation != nullptr)
	{
		// Get the animation object for the arms mesh
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

	// Check that the character is valid, and has no weapon component yet
	if (CharacterWeakPtr == nullptr || CharacterWeakPtr->GetInstanceComponents().FindItemByClass<UWeaponComponent>())
	{
		return false;
	}

	// Attach the weapon to the First Person Character
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(CharacterWeakPtr->GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));

	if (FireWeaponAbilityClass && CharacterWeakPtr->GetAbilitySystemComponent())
	{
		const FGameplayAbilitySpec AbilitySpec(FireWeaponAbilityClass, 1, -1, this);
		FireAbilityHandle = CharacterWeakPtr->GetAbilitySystemComponent()->GiveAbility(AbilitySpec);
		FireWeaponAbility = Cast<UFireWeaponAbility>(AbilitySpec.Ability);
		check(FireWeaponAbility);
	}

	// Set up action bindings
	if (APlayerController* PlayerController = Cast<APlayerController>(CharacterWeakPtr->GetController()))
	{
		if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(CharacterWeakPtr))
		{
			PlayerCharacter->SetWeapon(this);	
		}
		
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// Set the priority of the mapping to 1, so that it overrides the Jump action with the Fire action when using touch input
			Subsystem->AddMappingContext(FireMappingContext, 1);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			// Fire
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &UWeaponComponent::Fire);
		}
	}

	return true;
}

void UWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// ensure we have a character owner
	if (CharacterWeakPtr != nullptr)
	{
		// remove the input mapping context from the Player Controller
		if (APlayerController* PlayerController = Cast<APlayerController>(CharacterWeakPtr->GetController()))
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				Subsystem->RemoveMappingContext(FireMappingContext);
			}
		}
	}

	// maintain the EndPlay call chain
	Super::EndPlay(EndPlayReason);
}