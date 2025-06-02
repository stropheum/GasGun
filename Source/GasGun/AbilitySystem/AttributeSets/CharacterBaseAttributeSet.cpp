// Copyright 2025 Dale "Stropheum" Diaz


#include "CharacterBaseAttributeSet.h"
#include "GameplayEffectExtension.h"

float UCharacterBaseAttributeSet::GetHealthPercent() const
{
	return GetHealth() / GetMaxHealth();
}

float UCharacterBaseAttributeSet::GetShieldPercent() const
{
	return GetShield() / GetMaxShield();
}

void UCharacterBaseAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}
	else if (Attribute == GetMaxHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}
	else if (Attribute == GetShieldAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxShield());
	}
	else if (Attribute == GetMaxShieldAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxShield());
	}
}

void UCharacterBaseAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
}

void UCharacterBaseAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, const float OldValue, const float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
}

void UCharacterBaseAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		const float NewHealth = GetHealth();
		const float Delta = NewHealth - Data.EvaluatedData.Magnitude;
		OnHealthChanged.Broadcast(NewHealth, Delta);
	}
	else if (Data.EvaluatedData.Attribute == GetShieldAttribute())
	{
		const float NewShield = GetShield();
		const float Delta = NewShield - Data.EvaluatedData.Magnitude;
		OnHealthChanged.Broadcast(NewShield, Delta);
	}
}