// Copyright 2025 Dale "Stropheum" Diaz

#include "Projectile.h"

#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

AProjectile::AProjectile() 
{
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);

	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	RootComponent = CollisionComp;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	InitialLifeSpan = 3.0f;
}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		float Mass = 100.f;
		if (RootComponent)
		{
			if (const UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(RootComponent); PrimitiveComponent->IsSimulatingPhysics())
			{
				// Mass = PrimitiveComponent->GetMass();
			}
		}
		
		if (OtherActor)
		{
			auto ActorRoot = Cast<UPrimitiveComponent>(OtherActor->GetRootComponent());
			if (ActorRoot && ActorRoot->Mobility == EComponentMobility::Movable && ActorRoot->IsSimulatingPhysics())
			{
				ActorRoot->AddImpulseAtLocation(GetVelocity() * Mass, GetActorLocation());
			}
		}
		else if (OtherComp->Mobility == EComponentMobility::Movable && OtherComp->IsSimulatingPhysics())
		{
			OtherComp->AddImpulseAtLocation(GetVelocity() * Mass, GetActorLocation());	
		}
		// Destroy();
	}
}

UGunComponent* AProjectile::GetOwningGun() const
{
	return OwningGun;
}

void AProjectile::OnSetOwningGunCalled_Implementation(UGunComponent* Gun) {}

void AProjectile::SetOwningGun(UGunComponent* Gun)
{
	OwningGun = Gun;
	OnSetOwningGunCalled(Gun);
}

void AProjectile::RegisterOwnerTagListener(UAbilitySystemComponent* Asc, FGameplayTag TagToRegister)
{
	Asc->RegisterGameplayTagEvent(TagToRegister, EGameplayTagEventType::NewOrRemoved)
	   .AddUObject(this, &AProjectile::OnTagChanged);
}

void AProjectile::OnTagChanged(const FGameplayTag Tag, const int32 NewCount)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Tag %s changed to %d"), *Tag.ToString(), NewCount));
}
