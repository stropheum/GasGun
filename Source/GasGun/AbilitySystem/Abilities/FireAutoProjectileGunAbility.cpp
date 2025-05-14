// Copyright 2025 Dale "Stropheum" Diaz


#include "FireAutoProjectileGunAbility.h"

#include "GasGun/AbilitySystem/Tasks/ProjectileAutoFireTask.h"
#include "GasGun/Characters/PlayerCharacter.h"
#include "GasGun/Guns/GunComponent.h"
#include "GasGun/Guns/Projectile.h"

bool UFireAutoProjectileGunAbility::CanActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags,
	FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UFireAutoProjectileGunAbility::ActivateAbility(
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
	
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(Actor);
	UGunComponent* Gun = PlayerCharacter->GetGun();
	checkf(Gun != nullptr, TEXT("Attempting to activate FireProjectileGunAbility when Gun == nullptr"));

	const bool bCommitted = CommitAbility(Handle, ActorInfo, ActivationInfo);
	
	AutoFireTask = UProjectileAutoFireTask::CreateAutoFireTask(
		this,
		ProjectileClass,
		PlayerCharacter,
		Gun,
		FireSound,
		FireAnimation,
		FireRate);
	AutoFireTask->ReadyForActivation();
}

void UFireAutoProjectileGunAbility::CancelAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const bool bReplicateCancelAbility)
{
	if (AutoFireTask)
	{
		AutoFireTask->EndTask();
		AutoFireTask = nullptr;
	}
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UFireAutoProjectileGunAbility::EndAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const bool bReplicateEndAbility,
	const bool bWasCancelled)
{
	if (AutoFireTask)
	{
		AutoFireTask->EndTask();
		AutoFireTask = nullptr;
	}
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
