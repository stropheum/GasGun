// Copyright 2025 Dale "Stropheum" Diaz

#pragma once

#include "CoreMinimal.h"
#include "FireGunAbility_Base.h"
#include "FireGunAbility_Beam.generated.h"

class UGunComponent;
class UNiagaraComponent;
class UNiagaraSystem;
class UNiagaraAsset;

/**
 * Base Gameplay Ability for firing beam-based weapons
 */
UCLASS()
class GASGUN_API UFireGunAbility_Beam : public UFireGunAbility_Base
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

	void InitializeNiagaraSystem() const;

protected:

	UFUNCTION(blueprintCallable, Category=Ability, meta=(AllowPrivateAccess=true))
	void SetBeamActive(bool BeamIsActive) const;
	
	UFUNCTION(BlueprintCallable, Category=Ability, meta=(AllowPrivateAccess=true))
	void PerformRaycast() const;

	UFUNCTION()
	void OnBeamTick(float DeltaTime);
	
	UPROPERTY(EditDefaultsOnly, Category=Niagara, meta=(AllowPrivateAccess=true))
	TObjectPtr<UNiagaraComponent> NiagaraEffect{};
	
	UPROPERTY(EditDefaultsOnly, Category=Niagara, meta=(AllowPrivateAccess=true))
	TObjectPtr<UNiagaraSystem> NiagaraSystemAsset{};

	UPROPERTY(EditDefaultsOnly, Category=Abilities, meta=(AllowPrivateAccess=true))
	float BasePower;

	UPROPERTY()
	TObjectPtr<AActor> OwningActor{};

	UPROPERTY()
	TObjectPtr<UGunComponent> OwningGun{};
};
