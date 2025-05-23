// Copyright 2025 Dale "Stropheum" Diaz


#include "FireGunAbility_Flechette.h"

#include "GasGun/AbilitySystem/Tasks/AbilityTask_WaitStartFalling.h"
#include "GasGun/Characters/PlayerCharacter.h"
#include "GasGun/Guns/GunComponent.h"
#include "GasGun/Guns/Projectiles/FlechetteProjectile.h"
#include "GasGun/Guns/Projectiles/Projectile.h"

UFireGunAbility_Flechette::UFireGunAbility_Flechette()
{
}

void UFireGunAbility_Flechette::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!ActorInfo->OwnerActor.IsValid())
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
        return;
    }

    AActor* Owner = ActorInfo->OwnerActor.Get();
    const APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(Owner);
    if (!PlayerCharacter)
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
        return;
    }

    const UGunComponent* Gun = PlayerCharacter->GetGun();
    checkf(Gun != nullptr, TEXT("Attempting to activate FireProjectileGunAbility when Gun == nullptr"));

    const TTuple<FVector, FRotator> SpawnLocationRotation = Gun->GetProjectileSpawnPositionRotation();

    FActorSpawnParameters ActorSpawnParams;
    ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
    ActorSpawnParams.Owner = Owner;

    AFlechetteProjectile* Projectile = GetWorld()->SpawnActorDeferred<AFlechetteProjectile>(
        FlechetteProjectileClass,
        FTransform(SpawnLocationRotation.Value, SpawnLocationRotation.Key),
        ActorSpawnParams.Owner
    );

    if (!Projectile)
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
        return;
    }

    Projectile->FinishSpawning(FTransform(SpawnLocationRotation.Value, SpawnLocationRotation.Key));

    UAbilityTask_WaitStartFalling* WaitTask = UAbilityTask_WaitStartFalling::OnStartFallingEvent(this, TEXT("WaitStartFalling"), Projectile);
    if (WaitTask)
    {
        WaitTask->StartFallingEventReceived.AddDynamic(this, &UFireGunAbility_Flechette::OnProjectileStartFalling);
        WaitTask->ReadyForActivation();
    }
    else
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
        return;
    }

    // Keep the ability active until the task completes
    // Alternatively, end it here if you don't need to wait
    // EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UFireGunAbility_Flechette::OnProjectileStartFalling(AFlechetteProjectile* Projectile)
{
	
}
