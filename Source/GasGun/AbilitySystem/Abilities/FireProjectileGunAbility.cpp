// Copyright 2025 Dale "Stropheum" Diaz


#include "FireProjectileGunAbility.h"

#include "GasGun/Characters/PlayerCharacter.h"
#include "GasGun/Guns/GunComponent.h"
#include "GasGun/Guns/Projectile.h"
#include "Kismet/GameplayStatics.h"

bool UFireProjectileGunAbility::CanActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags,
	FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UFireProjectileGunAbility::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	check(ProjectileClass);

	UWorld* const World = GetWorld();
	check(World);

	const TWeakObjectPtr<AActor> Actor = ActorInfo->AvatarActor;
	checkf(Actor.IsValid(), TEXT("Actor is invalid when attempting to activate FireProjectileGunAbility"));
	checkf(FireAnimation != nullptr, TEXT("FireAnimation == nullptr"));
	checkf(FireSound != nullptr, TEXT("FireSound == nullptr"));
	
	const APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(Actor);
	const UGunComponent* Gun = PlayerCharacter->GetGun();
	checkf(Gun != nullptr, TEXT("Attempting to activate FireProjectileGunAbility when Gun == nullptr"));
	
	CommitAbility(Handle, ActorInfo, ActivationInfo);

	const TTuple<FVector, FRotator> SpawnLocationRotation = Gun->GetProjectileSpawnPositionRotation();
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	World->SpawnActor<AProjectile>(ProjectileClass, SpawnLocationRotation.Key, SpawnLocationRotation.Value, ActorSpawnParams);
	
	UGameplayStatics::PlaySoundAtLocation(this, FireSound, SpawnLocationRotation.Key);
	if (UAnimInstance* AnimInstance = PlayerCharacter->GetMesh1P()->GetAnimInstance(); AnimInstance != nullptr)
	{
		AnimInstance->Montage_Play(FireAnimation, 1.f);
	}
	// EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UFireProjectileGunAbility::CancelAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UFireProjectileGunAbility::EndAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const bool bReplicateEndAbility,
	const bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
