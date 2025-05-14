// Copyright 2025 Dale "Stropheum" Diaz


#include "ProjectileAutoFireTAsk.h"
#include "GasGun/Characters/PlayerCharacter.h"
#include "GasGun/Guns/GunComponent.h"
#include "GasGun/Guns/Projectile.h"
#include "Kismet/GameplayStatics.h"
#include "Abilities/Tasks/AbilityTask.h"

UProjectileAutoFireTask* UProjectileAutoFireTask::CreateAutoFireTask(
	UGameplayAbility* OwningAbility,
	TSubclassOf<AProjectile> ProjectileClass,
	APlayerCharacter* PlayerCharacter,
	UGunComponent* Gun,
	USoundBase* FireSound,
	UAnimMontage* FireAnimation,
	const float FireRate)
{
	UProjectileAutoFireTask* Task = NewAbilityTask<UProjectileAutoFireTask>(OwningAbility);
	Task->ProjectileClass = ProjectileClass;
	Task->PlayerCharacter = PlayerCharacter;
	Task->Gun = Gun;
	Task->FireSound = FireSound;
	Task->FireAnimation = FireAnimation;
	Task->FireRate = FireRate;
	Task->TimeSinceLastShot = 0.f;
	Task->bIsFiring = true;
	return Task;
}

void UProjectileAutoFireTask::Activate()
{
	Super::Activate();
	bTickingTask = true;
}

void UProjectileAutoFireTask::TickTask(const float DeltaTime)
{
	Super::TickTask(DeltaTime);

	if (!bIsFiring || !IsValid(PlayerCharacter) || !IsValid(Gun))
	{
		EndTask();
		return;
	}

	TimeSinceLastShot += DeltaTime;
	if (TimeSinceLastShot >= FireRate)
	{
		FireProjectile();
		TimeSinceLastShot = 0.f;
	}
}

void UProjectileAutoFireTask::FireProjectile() const
{
	if (UWorld* World = PlayerCharacter->GetWorld())
	{
		const TTuple<FVector, FRotator> SpawnLocationRotation = Gun->GetProjectileSpawnPositionRotation();
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
		World->SpawnActor<AProjectile>(ProjectileClass, SpawnLocationRotation.Key, SpawnLocationRotation.Value, ActorSpawnParams);

		UGameplayStatics::PlaySoundAtLocation(this, FireSound, SpawnLocationRotation.Key);
		if (UAnimInstance* AnimInstance = PlayerCharacter->GetMesh1P()->GetAnimInstance())
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}

void UProjectileAutoFireTask::OnDestroy(const bool bInOwnerFinished)
{
	bIsFiring = false;
	Super::OnDestroy(bInOwnerFinished);
}