// Copyright 2025 Dale "Stropheum" Diaz


#include "FireGunAbility_Projectile.h"

#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "GasGun/Characters/PlayerCharacter.h"
#include "GasGun/Guns/GunComponent.h"
#include "GasGun/Guns/Projectiles/Projectile.h"
#include "Kismet/GameplayStatics.h"

void UFireGunAbility_Projectile::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	GetWorld()->GetTimerManager().SetTimer(FireRateTimer, this, &UFireGunAbility_Projectile::Fire, FireTickRate, bIsAuto, 0.0f);
}

void UFireGunAbility_Projectile::CancelAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UFireGunAbility_Projectile::EndAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const bool bReplicateEndAbility,
	const bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

bool UFireGunAbility_Projectile::CanActivateAbility(
	const FGameplayAbilitySpecHandle Handle, 
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags,
	FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UFireGunAbility_Projectile::Fire()
{
	Super::Fire();

	check(ProjectileClass);

	UWorld* const World = GetWorld();
	check(World);

	const FGameplayAbilityActorInfo ActorInfo = GetActorInfo();
	const TWeakObjectPtr<AActor> Actor = ActorInfo.AvatarActor;
	checkf(Actor.IsValid(), TEXT("Actor is invalid when attempting to activate FireProjectileGunAbility"));
	checkf(FireAnimation != nullptr, TEXT("FireAnimation == nullptr"));
	checkf(FireSound != nullptr, TEXT("FireSound == nullptr"));
	
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(Actor);
	UGunComponent* Gun = PlayerCharacter->GetGun();
	checkf(Gun != nullptr, TEXT("Attempting to activate FireProjectileGunAbility when Gun == nullptr"));
	
	const TTuple<FVector, FRotator> SpawnLocationRotation = Gun->GetProjectileSpawnPositionRotation();
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	AProjectile* FiredProjectile = World->SpawnActor<AProjectile>(ProjectileClass, SpawnLocationRotation.Key, SpawnLocationRotation.Value, ActorSpawnParams);

	if (FiredProjectile)
	{
		FiredProjectile->SetOwningGun(Gun);
		auto ProjectileCollisionComp = FiredProjectile->GetCollisionComp();
		auto PlayerCapsule = PlayerCharacter->GetCapsuleComponent();
		if (ProjectileCollisionComp && PlayerCapsule)
		{
			ProjectileCollisionComp->IgnoreActorWhenMoving(PlayerCharacter, true);
			PlayerCapsule->IgnoreActorWhenMoving(FiredProjectile, true);
		}
	}
	
	UGameplayStatics::PlaySoundAtLocation(this, FireSound, SpawnLocationRotation.Key);
	if (UAnimInstance* AnimInstance = PlayerCharacter->GetMesh1P()->GetAnimInstance(); AnimInstance != nullptr)
	{
		AnimInstance->Montage_Play(FireAnimation, 1.f);
	}
}