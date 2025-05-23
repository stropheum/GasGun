// Copyright 2025 Dale "Stropheum" Diaz

#pragma once

#include "CoreMinimal.h"
#include "FireGunAbility_Projectile.h"
#include "FireGunAbility_Flechette.generated.h"

class AFlechetteProjectile;
/**
 * 
 */
UCLASS()
class GASGUN_API UFireGunAbility_Flechette : public UFireGunAbility_Projectile
{
	GENERATED_BODY()

public:
	UFireGunAbility_Flechette();

protected:
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

	UFUNCTION()
	void OnProjectileStartFalling(AFlechetteProjectile* Projectile);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Flechette")
	TSubclassOf<AProjectile> FlechetteProjectileClass{};

	UPROPERTY(EditDefaultsOnly, Category = "Flechette")
	int BurstCount = 50;

	UPROPERTY()
	int ProjectileSpawnCount = 0;

	UPROPERTY()
	TArray<AProjectile*> ProjectileActors{};
};
