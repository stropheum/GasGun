// Copyright 2025 Dale "Stropheum" Diaz


#include "FlechetteProjectile.h"

#include "GameFramework/ProjectileMovementComponent.h"


AFlechetteProjectile::AFlechetteProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AFlechetteProjectile::BeginPlay()
{
	Super::BeginPlay();

	GetProjectileMovement()->bRotationFollowsVelocity = false;
	GetProjectileMovement()->bRotationRemainsVertical = true;
}

void AFlechetteProjectile::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	AddActorLocalRotation(FRotator(0.f, RotationSpeed * DeltaTime, 0.f));
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, FString::Printf(TEXT("Velocity: %s"), *GetActorRotation().ToString()));
	CheckIsFalling();
}

void AFlechetteProjectile::CheckIsFalling()
{
	if (bIsFallingTriggered) { return; }

	const FVector CurrentVelocity = GetVelocity();
	if (CurrentVelocity.Z > 0.f)
	{
		return;
	}

	bIsFallingTriggered = true;
	GetWorld()->GetTimerManager().SetTimer(FireRateTimer, this, &AFlechetteProjectile::Fire, FireRate, true, 0.0f);
}

void AFlechetteProjectile::Fire()
{
	if (RoundsFired < AmmoCapacity)
	{
		RoundsFired++;
		
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.Owner = this;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		const auto World = GetWorld();
		AProjectile* SubProjectile = World->SpawnActor<AProjectile>(SubProjectileClass, GetActorLocation(), GetActorRotation(), ActorSpawnParams);

		SubProjectile->GetProjectileMovement()->Velocity = GetActorForwardVector() * SubProjectileVelocity;
		SubProjectiles.Add(SubProjectile);
	}
}

