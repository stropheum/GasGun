// Copyright 2025 Dale "Stropheum" Diaz

#pragma once

#include "CoreMinimal.h"
#include "FireGunAbility_Base.h"
#include "FireGunAbility_Projectile.generated.h"

/**
 * Base Gameplay Ability for firing projectile-based weapons
 */
UCLASS()
class GASGUN_API UFireGunAbility_Projectile : public UFireGunAbility_Base
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

	virtual void Fire() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=GameplayAbilities, meta=(AllowPrivateAccess=true))
	bool bIsAuto{};

	UPROPERTY(BlueprintReadOnly, Category=GameplayAbilities, meta=(AllowPrivateAccess=true))
	bool bHasFired{};
	
	FTimerHandle FireRateTimer;
};
