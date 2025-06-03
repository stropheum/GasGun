// Copyright 2025 Dale "Stropheum" Diaz

#include "Projectile.h"

#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "NativeGameplayTags.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "GasGun/AbilitySystem/AttributeSets/ProjectileAttributeSet.h"
#include "GasGun/AbilitySystem/Data/NativeGameplayTags.h"
#include "GasGun/Characters/CharacterBase.h"
#include "Net/UnrealNetwork.h"

AProjectile::AProjectile() 
{
	AttributeSet = CreateDefaultSubobject<UProjectileAttributeSet>("AttributeSet");

	bReplicates = true;
	
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->SetIsReplicated(true);
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
	
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	RootComponent = CollisionComp;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->SetIsReplicated(true);
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;

	InitialLifeSpan = 3.0f;
}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		if (OtherActor)
		{
			OnActorHit(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);
		}
		else if (OtherComp->Mobility == EComponentMobility::Movable && OtherComp->IsSimulatingPhysics())
		{
			OtherComp->AddImpulseAtLocation(GetVelocity() * Mass, GetActorLocation());	
		}
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

void AProjectile::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AProjectile, AttributeSet);
	DOREPLIFETIME(AProjectile, CollisionComp);
	DOREPLIFETIME(AProjectile, ProjectileMovement);
	DOREPLIFETIME(AProjectile, OwningGun);
	DOREPLIFETIME(AProjectile, DamageEffectClass);
}

void AProjectile::OnTagChanged(const FGameplayTag Tag, const int32 NewCount)
{
	
}

void AProjectile::OnActorHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (const ACharacterBase* Character = Cast<ACharacterBase>(OtherActor))
	{
		if (UAbilitySystemComponent* HitCharacterAcs = Character->GetAbilitySystemComponent();
			ensureMsgf(HitCharacterAcs, TEXT("No AbilitySystemComponent set for %s"), *GetName()))
		{
			FGameplayEffectContextHandle EffectContext = HitCharacterAcs->MakeEffectContext();
			EffectContext.AddSourceObject(this);
			EffectContext.AddHitResult(Hit);

			if (!DamageEffectClass)
			{
				return;
			}
			
			if (const FGameplayEffectSpecHandle SpecHandle = HitCharacterAcs->MakeOutgoingSpec(DamageEffectClass, 1, EffectContext);
				ensureMsgf(SpecHandle.IsValid(), TEXT("SpecHandle == nullptr for %s"), *GetName()))
			{
				const float DamageValue = AttributeSet->GetDamage();
				const FGameplayTag ProjectileDamageTag = NativeGameplayTags::Projectile::TAG_DamageType_DirectDamage.GetTag();
				SpecHandle.Data->SetSetByCallerMagnitude(ProjectileDamageTag, DamageValue);
				
				HitCharacterAcs->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
			}

			if (OtherComp->IsSimulatingPhysics())
			{
				OtherComp->AddImpulseAtLocation(GetVelocity() * Mass, GetActorLocation());	
			}
		}	
	}

	if (UPrimitiveComponent* ActorRoot = Cast<UPrimitiveComponent>(OtherActor->GetRootComponent()); ActorRoot && ActorRoot->Mobility == EComponentMobility::Movable && ActorRoot->IsSimulatingPhysics())
	{
		ActorRoot->AddImpulseAtLocation(GetVelocity() * Mass, GetActorLocation());
	}
}
