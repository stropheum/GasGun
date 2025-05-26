// Copyright 2025 Dale "Stropheum" Diaz

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "FireGunAbility_Base.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFireGunTick);

/**
 * Base Gameplay Ability for firing all weapons
 */
UCLASS()
class GASGUN_API UFireGunAbility_Base : public UGameplayAbility
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

	virtual void CancelAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		bool bReplicateCancelAbility) override;

	virtual void EndAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		bool bReplicateEndAbility,
		bool bWasCancelled) override;

	virtual bool CanActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayTagContainer* SourceTags = nullptr,
		const FGameplayTagContainer* TargetTags = nullptr,
		FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	UFUNCTION(BlueprintCallable, Category = "Ability Tasks")
	virtual void Fire();

protected:
	
	UPROPERTY(EditDefaultsOnly, Category=Projectile, meta=(AllowPrivateAccess=true))
	TSubclassOf<class AProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay, meta=(AllowPrivateAccess=true))
	TObjectPtr<USoundBase> FireSound{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay, meta=(AllowPrivateAccess=true))
	TObjectPtr<UAnimMontage> FireAnimation{};

	UPROPERTY(BlueprintAssignable, Category=GameplayAbilities, meta=(AllowPrivateAccess=true))
	FFireGunTick OnFireTick{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=GameplayAbilities, meta=(AllowPrivateAccess=true))
	float FireTickRate = 0.1f;
};
