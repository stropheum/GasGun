// Copyright 2025 Dale "Stropheum" Diaz

#pragma once

#include "CoreMinimal.h"
#include "FireGunAbility_Base.h"
#include "Abilities/GameplayAbility.h"
#include "FireAutoProjectileGunAbility.generated.h"

class UProjectileAutoFireTask;
/**
 * Gameplay ability to handle firing for automatic weapons that shoot physical projectiles
 */
UCLASS()
class GASGUN_API UFireAutoProjectileGunAbility : public UFireGunAbility_Base
{
	GENERATED_BODY()

public:
	virtual bool CanActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayTagContainer* SourceTags = nullptr,
		const FGameplayTagContainer* TargetTags = nullptr,
		FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

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

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Abilities", meta = (AllowPrivateAccess = "true"))
	float FireRate = 0.1f;

	UPROPERTY()
	UProjectileAutoFireTask* AutoFireTask{};
};
