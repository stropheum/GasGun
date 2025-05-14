// Copyright 2025 Dale "Stropheum" Diaz

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "ProjectileAutoFireTask.generated.h"

class UGunComponent;
class APlayerCharacter;
class AProjectile;
/**
 * A task to handle automatic weapon projectile spawning
 */
UCLASS()
class GASGUN_API UProjectileAutoFireTask : public UAbilityTask
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "GameplayTasks", meta = (BlueprintInternalUseOnly = "true"))
	static UProjectileAutoFireTask* CreateAutoFireTask(
		UGameplayAbility* OwningAbility,
		TSubclassOf<AProjectile> ProjectileClass,
		APlayerCharacter* PlayerCharacter,
		UGunComponent* Gun,
		USoundBase* FireSound,
		UAnimMontage* FireAnimation, float FireRate);

	UFUNCTION()
	virtual void Activate() override;

	UFUNCTION()
	virtual void TickTask(float DeltaTime) override;

	UFUNCTION()
	virtual void OnDestroy(bool bInOwnerFinished) override;

protected:
	void FireProjectile() const;

	UPROPERTY()
	TSubclassOf<AProjectile> ProjectileClass;

	UPROPERTY()
	APlayerCharacter* PlayerCharacter;

	UPROPERTY()
	UGunComponent* Gun;

	UPROPERTY()
	USoundBase* FireSound;

	UPROPERTY()
	UAnimMontage* FireAnimation;

	float FireRate;
	float TimeSinceLastShot;
	bool bIsFiring;
};
