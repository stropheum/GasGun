// Copyright 2025 Dale "Stropheum" Diaz


#include "StickyProjectile.h"

#include "Components/SphereComponent.h"


// Sets default values
AStickyProjectile::AStickyProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AStickyProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

void AStickyProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::OnHit(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);
	// TODO: Sticky bomb logic here
}

// Called every frame
void AStickyProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

