// Copyright 2025 Dale "Stropheum" Diaz

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "FlechetteProjectile.generated.h"

class UGunComponent;

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

protected:
	UFUNCTION()
	void CheckIsFalling();

	UFUNCTION()
	void Fire();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FlechetteProjectile", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AProjectile> SubProjectileClass{};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FlechetteProjectile", meta = (AllowPrivateAccess = "true"))
	float RotationSpeed = 1000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FlechetteProjectile", meta = (AllowPrivateAccess = "true"))
	float FireRate = 0.1f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FlechetteProjectile", meta = (AllowPrivateAccess = "true"))
	float SubProjectileVelocity = 1000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FlechetteProjectile", meta = (AllowPrivateAccess = "true"))
	float AngleSpread = 360.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FlechetteProjectile", meta = (AllowPrivateAccess = "true"))
	float MinVerticalVelocityTrigger = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FlechetteProjectile", meta = (AllowPrivateAccess = "true"))
	int RoundsPerTick = 4;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FlechetteProjectile", meta = (AllowPrivateAccess = "true"))
	int AmmoCapacity = 50;

	UPROPERTY(BlueprintReadOnly, Category = "FlechetteProjectile", meta = (AllowPrivateAccess = "true"))
	bool bIsFallingTriggered = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FlechetteProjectile", meta = (AllowPrivateAccess = "true"))
	USoundBase* SubProjectileFireSound{};

	UPROPERTY()
	TArray<AProjectile*> SubProjectiles{};

	FTimerHandle FireRateTimer;
	int RoundsFired = 0;
	FVector LastTickVelocity{};
};
