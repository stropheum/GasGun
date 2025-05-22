// Copyright 2025 Dale "Stropheum" Diaz

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "FlechetteProjectile.generated.h"

UCLASS()
class GASGUN_API AFlechetteProjectile : public AProjectile
{
	GENERATED_BODY()

public:
	AFlechetteProjectile();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
