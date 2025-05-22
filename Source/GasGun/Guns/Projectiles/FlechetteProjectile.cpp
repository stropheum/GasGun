// Copyright 2025 Dale "Stropheum" Diaz


#include "FlechetteProjectile.h"


AFlechetteProjectile::AFlechetteProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AFlechetteProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFlechetteProjectile::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

