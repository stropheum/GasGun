// Copyright 2025 Dale "Stropheum" Diaz


#include "FlechetteProjectile.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"


AFlechetteProjectile::AFlechetteProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AFlechetteProjectile::BeginPlay()
{
	Super::BeginPlay();

	check(ProjectileAttributes.SubProjectileFireSound);

	ProjectileMovement->bRotationFollowsVelocity = false;
	ProjectileMovement->bRotationRemainsVertical = true;
	LastTickVelocity = ProjectileMovement->Velocity;
	SetActorRotation(FRotator::ZeroRotator);
}

void AFlechetteProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
}

void AFlechetteProjectile::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	AddActorLocalRotation(FRotator(0.f, ProjectileAttributes.RotationSpeed * DeltaTime, 0.f));
	CheckIsFalling();
}

void AFlechetteProjectile::CheckIsFalling()
{
	if (ProjectileAttributes.bIsFallingTriggered) { return; }
	
	if (GetVelocity().Z > ProjectileAttributes.MinVerticalVelocityTrigger)
	{
		return;
	}

	ProjectileAttributes.bIsFallingTriggered = true;
	GetWorld()->GetTimerManager().SetTimer(FireRateTimer, this, &AFlechetteProjectile::Fire, ProjectileAttributes.FireRate, true, 0.0f);
}

void AFlechetteProjectile::Fire()
{
	if (RoundsFired >= ProjectileAttributes.AmmoCapacity)
	{
		Destroy();
		return;
	}
	
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.Owner = this;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	
	const auto World = GetWorld();
	const FVector SpawnLocation = GetActorLocation();
	const FRotator BaseRotation = GetActorRotation();
	const int32 RoundsToFire = FMath::Min(ProjectileAttributes.RoundsPerTick, ProjectileAttributes.AmmoCapacity - RoundsFired);
	const float HalfSpreadAngle = ProjectileAttributes.AngleSpread * 0.5f;
	const float AngleIncrement = (RoundsToFire > 1) ? ProjectileAttributes.AngleSpread / static_cast<float>(RoundsToFire - 1) : 0.0f;
	
	for (int32 i = 0; i < RoundsToFire; ++i)
	{
		const float AngleOffset = -HalfSpreadAngle + (AngleIncrement * i);
		
		FRotator ProjectileRotation = BaseRotation;
		ProjectileRotation.Yaw += AngleOffset;
		
		AProjectile* SubProjectile = World->SpawnActor<AProjectile>(ProjectileAttributes.SubProjectileClass, SpawnLocation, ProjectileRotation, ActorSpawnParams);

		UPrimitiveComponent* FlechetteCollision = Cast<UPrimitiveComponent>(GetRootComponent());
		UPrimitiveComponent* SubProjectileCollision = Cast<UPrimitiveComponent>(SubProjectile->GetRootComponent());

		if (FlechetteCollision && SubProjectileCollision)
		{
			FlechetteCollision->IgnoreActorWhenMoving(SubProjectile, true);
			SubProjectileCollision->IgnoreActorWhenMoving(this, true);
		}
		
		// const FVector Direction = ProjectileRotation.Vector();
		// SubProjectile->GetProjectileMovement()->MaxSpeed = 0.f;
		// SubProjectile->GetProjectileMovement()->Velocity = FVector::ZeroVector;
		
		SubProjectiles.Add(SubProjectile);
		
		RoundsFired++;
	}
	
	UGameplayStatics::PlaySoundAtLocation(this, ProjectileAttributes.SubProjectileFireSound, GetActorLocation());
}

