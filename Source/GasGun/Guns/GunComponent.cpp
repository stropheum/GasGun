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
#include "GasGun/AbilitySystem/Abilities/FireGunAbility_Base.h"
#include "Net/UnrealNetwork.h"

UGunComponent::UGunComponent()
{
	SetIsReplicatedByDefault(true);
	MuzzleOffset = FVector(56.5f, 14.25f, 11.3f);
}

void UGunComponent::ActivateFireAbility()
{
	if (!CharacterWeakPtr.IsValid() || !CharacterWeakPtr->GetController())
	{
		return;
	}
	
	UAbilitySystemComponent* Asc = CharacterWeakPtr->GetAbilitySystemComponent();
	if (!Asc || !FireAbilityHandle.IsValid() || !FireGunAbility)
	{
		return;
	}

	FGameplayAbilitySpec* AbilitySpec = Asc->FindAbilitySpecFromHandle(FireAbilityHandle);
	if (!AbilitySpec)
	{
		return;
	}

	Asc->TryActivateAbility(FireAbilityHandle);
}

void UGunComponent::DeactivateFireAbility()
{
	if (!FireAbilityHandle.IsValid() || !FireGunAbility)
	{
		UE_LOG(LogTemp, Warning, TEXT("No FireRifleAbility set"));
		return;
	}

	UAbilitySystemComponent* Asc = CharacterWeakPtr->GetAbilitySystemComponent();
	Asc->CancelAbilityHandle(FireAbilityHandle);
}

TTuple<FVector, FRotator> UGunComponent::GetProjectileSpawnPositionRotation() const
{
	const APlayerController* PlayerController = Cast<APlayerController>(CharacterWeakPtr->GetController());
	const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
	const FVector SpawnPosition = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);
	return {SpawnPosition, SpawnRotation};
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

	if (CharacterWeakPtr->HasAuthority() && FireWeaponAbilityClass && CharacterWeakPtr->GetAbilitySystemComponent())
	{
		if (CharacterWeakPtr->HasAuthority())
		{
			UAbilitySystemComponent* ASC = CharacterWeakPtr->GetAbilitySystemComponent();
			const FGameplayAbilitySpec AbilitySpec(FireWeaponAbilityClass, 1, -1, this);
			FireAbilityHandle = ASC->GiveAbility(AbilitySpec);

			CharacterWeakPtr->ForceNetUpdate();
            
			FireGunAbility = Cast<UFireGunAbility_Base>(AbilitySpec.Ability);
		}
	}

	if (const APlayerController* PlayerController = Cast<APlayerController>(CharacterWeakPtr->GetController()))
	{
		if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(CharacterWeakPtr))
		{
			PlayerCharacter->SetGun(this);
		}

		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(FireMappingContext, 1);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &UGunComponent::ActivateFireAbility);
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &UGunComponent::DeactivateFireAbility);
		}
	}

	return true;
}

TWeakObjectPtr<APlayerCharacter> UGunComponent::GetOwningPlayerWeakPtr() const
{
	return CharacterWeakPtr;
}

void UGunComponent::BeginPlay()
{
	Super::BeginPlay();
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

void UGunComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UGunComponent, FireWeaponAbilityClass);
	DOREPLIFETIME(UGunComponent, MuzzleOffset);
	DOREPLIFETIME(UGunComponent, FireMappingContext);
	DOREPLIFETIME(UGunComponent, FireAction);
	DOREPLIFETIME_CONDITION_NOTIFY(UGunComponent, FireAbilityHandle, COND_None, REPNOTIFY_Always);
}

void UGunComponent::OnRep_FireAbilityHandle()
{
	if (CharacterWeakPtr.IsValid() && CharacterWeakPtr->GetAbilitySystemComponent())
	{
		FGameplayAbilitySpec* AbilitySpec = CharacterWeakPtr->GetAbilitySystemComponent()->FindAbilitySpecFromHandle(FireAbilityHandle);
		if (AbilitySpec)
		{
			FireGunAbility = Cast<UFireGunAbility_Base>(AbilitySpec->Ability);
			UE_LOG(LogTemp, Warning, TEXT("OnRep_FireAbilityHandle: Set FireGunAbility %s"), 
				FireGunAbility ? TEXT("Successfully") : TEXT("Failed"));
		}
	}

}
