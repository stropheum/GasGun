// Copyright 2025 Dale "Stropheum" Diaz

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "StickyProjectile.generated.h"

UCLASS()
class GASGUN_API AStickyProjectile : public AProjectile
{
	GENERATED_BODY()

public:
	AStickyProjectile();

protected:
	virtual void BeginPlay() override;

	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;

public:
	virtual void Tick(float DeltaTime) override;
};
