// Copyright 2025 Dale "Stropheum" Diaz


#include "FireGunAbility.h"

bool UFireGunAbility::CanActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags,
	FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UFireGunAbility::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, TEXT("UFireGunAbility::ActivateAbility"));

	//TODO: don't do this here but in each subclass specify how it's going to fire. This is reference code from GunComponent.cpp in the starter project
	// if (ProjectileClass != nullptr)
	// {
	// 	UWorld* const World = GetWorld();
	// 	if (World != nullptr)
	// 	{
	// 		APlayerController* PlayerController = Cast<APlayerController>(CharacterWeakPtr->GetController());
	// 		const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
	// 		// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
	// 		const FVector SpawnLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);
	//
	// 		FActorSpawnParameters ActorSpawnParams;
	// 		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	// 		World->SpawnActor<AProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
	// 	}
	// }
	//
	// if (FireSound != nullptr)
	// {
	// 	UGameplayStatics::PlaySoundAtLocation(this, FireSound, CharacterWeakPtr->GetActorLocation());
	// }
	//
	// if (FireAnimation != nullptr)
	// {
	// 	UAnimInstance* AnimInstance = CharacterWeakPtr->GetMesh1P()->GetAnimInstance();
	// 	if (AnimInstance != nullptr)
	// 	{
	// 		AnimInstance->Montage_Play(FireAnimation, 1.f);
	// 	}
	// }
}

void UFireGunAbility::CancelAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, TEXT("UFireGunAbility::CancelAbility"));
}

void UFireGunAbility::EndAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const bool bReplicateEndAbility,
	const bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, TEXT("UFireGunAbility::EndAbility"));
}
