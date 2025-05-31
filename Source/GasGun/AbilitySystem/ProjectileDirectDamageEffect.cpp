// Copyright 2025 Dale "Stropheum" Diaz

#include "GameplayEffect.h"
#include "AbilitySystemGlobals.h"
#include "GameplayEffectComponents/AssetTagsGameplayEffectComponent.h"
#include "ProjectileDirectDamageEffect.h"
#include "Data/NativeGameplayTags.h"
#include "EffectCalculations/CharacterDamageExecution.h"


UProjectileDirectDamageEffect::UProjectileDirectDamageEffect()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;
}

void UProjectileDirectDamageEffect::PostInitProperties()
{
	Super::PostInitProperties();

	DurationMagnitude = FGameplayEffectModifierMagnitude();
	
	FGameplayEffectExecutionDefinition ExecutionDef;
	ExecutionDef.CalculationClass = UCharacterDamageExecution::StaticClass();
	Executions.Add(ExecutionDef);

	UAssetTagsGameplayEffectComponent& AssetTagComponent = FindOrAddComponent<UAssetTagsGameplayEffectComponent>();
	FInheritedTagContainer Tags = AssetTagComponent.GetConfiguredAssetTagChanges();
	Tags.AddTag(NativeGameplayTags::Projectile::TAG_DamageType_DirectDamage.GetTag());
	AssetTagComponent.SetAndApplyAssetTagChanges(Tags);

	StackingType = EGameplayEffectStackingType::AggregateByTarget;
	StackLimitCount = 1;
	bDenyOverflowApplication = true;
	StackDurationRefreshPolicy = EGameplayEffectStackingDurationPolicy::NeverRefresh;
	StackPeriodResetPolicy = EGameplayEffectStackingPeriodPolicy::NeverReset;
}