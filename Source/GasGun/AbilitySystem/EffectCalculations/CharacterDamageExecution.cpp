// Copyright 2025 Dale "Stropheum" Diaz


#include "CharacterDamageExecution.h"

#include "GasGun/AbilitySystem/AttributeSets/CharacterBaseAttributeSet.h"
#include "GasGun/AbilitySystem/AttributeSets/ProjectileAttributeSet.h"
#include "GasGun/AbilitySystem/Data/NativeGameplayTags.h"


struct FDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Shield);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Damage);

	FDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCharacterBaseAttributeSet, Shield, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCharacterBaseAttributeSet, Health, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UProjectileAttributeSet, Damage, Source, true);
	}
};

static const FDamageStatics& DamageStatics()
{
	static FDamageStatics DmgStatics;
	return DmgStatics;
}

UCharacterDamageExecution::UCharacterDamageExecution()
{
	RelevantAttributesToCapture.Add(DamageStatics().ShieldDef);
	RelevantAttributesToCapture.Add(DamageStatics().HealthDef);
	RelevantAttributesToCapture.Add(DamageStatics().DamageDef);
}

void UCharacterDamageExecution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;
	
	float CurrentShield = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ShieldDef, EvaluationParameters, CurrentShield);
	CurrentShield = FMath::Max<float>(CurrentShield, 0.0f);

	float CurrentHealth = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().HealthDef, EvaluationParameters, CurrentHealth);
	CurrentHealth = FMath::Max<float>(CurrentHealth, 0.0f);
	
	float DamageAmount = 0.0f;
	const FGameplayTag ProjectileDamageTag = NativeGameplayTags::Projectile::TAG_DamageType_DirectDamage.GetTag();
	if (Spec.SetByCallerTagMagnitudes.Contains(ProjectileDamageTag))
	{
		DamageAmount = Spec.GetSetByCallerMagnitude(ProjectileDamageTag);
	}
	DamageAmount = FMath::Max<float>(DamageAmount, 0.0f);

	
	const float ShieldDamage = FMath::Min(CurrentShield, DamageAmount);
	const float HealthDamage = DamageAmount - ShieldDamage;

	if (ShieldDamage > 0.0f)
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics().ShieldProperty, EGameplayModOp::Additive, -ShieldDamage));
	}

	if (HealthDamage > 0.0f)
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics().HealthProperty, EGameplayModOp::Additive, -HealthDamage));
	}

	float NewShield = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ShieldDef, EvaluationParameters, NewShield);
	NewShield = FMath::Max<float>(NewShield, 0.0f);

	float NewHealth = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().HealthDef, EvaluationParameters, NewHealth);
	NewHealth = FMath::Max<float>(NewHealth, 0.0f);

	UE_LOG(LogTemp, Log, TEXT("Damage Distribution - Total: %f, h:%f/s:%f | Remaining: h:%f/s:%f"), DamageAmount, HealthDamage, ShieldDamage, NewHealth, NewShield);
}