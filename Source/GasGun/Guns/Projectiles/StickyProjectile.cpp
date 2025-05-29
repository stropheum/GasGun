// Copyright 2025 Dale "Stropheum" Diaz


#include "StickyProjectile.h"

#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/RadialForceActor.h"
#include "PhysicsEngine/RadialForceComponent.h"


AStickyProjectile::AStickyProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AStickyProjectile::OnTagChanged(const FGameplayTag Tag, int32 NewCount)
{
	Super::OnTagChanged(Tag, NewCount);

	const FGameplayTag DetonationTag = FGameplayTag::RequestGameplayTag(FName("Weapon.Projectile.StickyBomb.Detonate"));
	if (Tag == DetonationTag && NewCount >= 0)
	{
		Detonate();
	}
}

void AStickyProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void AStickyProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor != this && OtherComp)
	{
		AttachToActor(OtherActor, FAttachmentTransformRules::KeepWorldTransform);

		if (ProjectileMovement)
		{
			ProjectileMovement->StopMovementImmediately();
			SetLifeSpan(0.f);
		}

		CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else if (OtherComp)
	{
		AttachToComponent(OtherComp, FAttachmentTransformRules::KeepWorldTransform);

		if (ProjectileMovement)
		{
			ProjectileMovement->StopMovementImmediately();
			SetLifeSpan(0.f);
		}

		CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
}

void AStickyProjectile::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AStickyProjectile::Detonate()
{
	const FVector Location = GetActorLocation();
    const FRotator Rotation = GetActorRotation();
    
    if (UWorld* World = GetWorld())
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        
        if (ARadialForceActor* RadialForceActor = World->SpawnActor<ARadialForceActor>(Location, Rotation, SpawnParams))
        {
            if (URadialForceComponent* ForceComponent = RadialForceActor->GetForceComponent())
            {
                ForceComponent->Radius = 1000.0f;         
                ForceComponent->ForceStrength = 500000.0f;
                ForceComponent->ImpulseStrength = 0.0f;   
                ForceComponent->bImpulseVelChange = false;
                ForceComponent->Falloff = RIF_Constant;   
                ForceComponent->bAutoActivate = true;     
            	
                ForceComponent->Activate();
                ForceComponent->FireImpulse();
            	
                FTimerHandle TimerHandle;
                World->GetTimerManager().SetTimer(TimerHandle, [RadialForceActor]
                {
                    if (IsValid(RadialForceActor))
                    {
                        RadialForceActor->Destroy();
                    }
                }, 0.1f, false);
            }
        }
    }
    
    if (ensureMsgf(DetonationNiagaraSystem, TEXT("No Detonation Niagara System set for %s"), *GetName()))
    {
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(
            GetWorld(),
            DetonationNiagaraSystem,
            Location,
            Rotation,
            FVector(1.f, 1.f, 1.f),
            true);
    }

    if (ensureMsgf(DetonationSound, TEXT("No Detonation Sound set for %s"), *GetName()))
    {
        UGameplayStatics::PlaySoundAtLocation(this, DetonationSound, Location);    
    }
    
    Destroy();

}
