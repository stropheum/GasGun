// Copyright 2025 Dale "Stropheum" Diaz

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class UGunComponent;
class USphereComponent;
class UProjectileMovementComponent;

USTRUCT(BlueprintType)
struct FFlechetteProjectileConfig
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FlechetteProjectileConfig" )
	TSubclassOf<class AProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FlechetteProjectileConfig")
	int Count;
	
};

UCLASS(config=Game)
class AProjectile : public AActor
{
	GENERATED_BODY()

public:
	
	AProjectile();

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	UGunComponent* GetOwningGun() const;

	UFUNCTION()
	void SetOwningGun(UGunComponent* Gun);

	USphereComponent* GetCollisionComp() const { return CollisionComp; }
	
	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

protected:
	
	UPROPERTY(VisibleDefaultsOnly, Category=Projectile)
	USphereComponent* CollisionComp{};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement{};

	UPROPERTY()
	UGunComponent* OwningGun{};
};

