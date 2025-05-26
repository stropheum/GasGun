// Copyright 2025 Dale "Stropheum" Diaz


#include "StickyProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"


AStickyProjectile::AStickyProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AStickyProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

void AStickyProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Super::OnHit(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);
	
	if (OtherActor && OtherActor != this && OtherComp)
	{
		AttachToActor(OtherActor, FAttachmentTransformRules::KeepWorldTransform);
		
		if (ProjectileMovement)
		{
			ProjectileMovement->StopMovementImmediately();
			SetLifeSpan(0.f);
		}
		
		CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	else if (OtherComp)
	{
		AttachToComponent(OtherComp, FAttachmentTransformRules::KeepWorldTransform);
		
		if (ProjectileMovement)
		{
			ProjectileMovement->StopMovementImmediately();
			SetLifeSpan(0.f);
		}
		
		CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AStickyProjectile::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

