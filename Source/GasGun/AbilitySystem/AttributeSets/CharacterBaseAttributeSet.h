// Copyright 2025 Dale "Stropheum" Diaz

#pragma once

#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "AttributeSetBase.h"
#include "CoreMinimal.h"
#include "CharacterBaseAttributeSet.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, float, NewHealth, float, Delta);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMaxHealthChanged, float, NewMaxHealth, float, Delta);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnShieldChanged, float, NewShield, float, Delta);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMaxShieldChanged, float, NewMaxShield, float, Delta);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnShieldRegenRateChanged, float, NewShieldRegenRate, float, Delta);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnShieldRegenDelayChanged, float, NewShieldRegenDelay, float, Delta);

UCLASS()
class GASGUN_API UCharacterBaseAttributeSet : public UAttributeSetBase
{
	GENERATED_BODY()

	static FName GetDamageTagName() { return FName("Damage.Amount"); }

	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnHealthChanged OnHealthChanged;
	
	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnHealthChanged OnMaxHealthChanged;
	
	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnShieldChanged OnShieldChanged;

	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnHealthChanged OnMaxShieldChanged;

	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnShieldChanged OnShieldRegenRateChanged;

	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnHealthChanged OnShieldRegenDelayChanged;
	
	UPROPERTY(BlueprintReadOnly, Category="Attributes", meta=(AllowPrivateAccess="true"))
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UCharacterBaseAttributeSet, Health)
	
	UPROPERTY(BlueprintReadOnly, Category="Attributes", meta=(AllowPrivateAccess="true"))
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UCharacterBaseAttributeSet, MaxHealth)
	
	UPROPERTY(BlueprintReadOnly, Category="Attributes", meta=(AllowPrivateAccess="true"))
	FGameplayAttributeData Shield;
	ATTRIBUTE_ACCESSORS(UCharacterBaseAttributeSet, Shield)
	
	UPROPERTY(BlueprintReadOnly, Category="Attributes", meta=(AllowPrivateAccess="true"))
	FGameplayAttributeData MaxShield;
	ATTRIBUTE_ACCESSORS(UCharacterBaseAttributeSet, MaxShield)
	
	UPROPERTY(BlueprintReadOnly, Category="Attributes", meta=(AllowPrivateAccess="true"))
	FGameplayAttributeData ShieldRegenDelay;
	ATTRIBUTE_ACCESSORS(UCharacterBaseAttributeSet, ShieldRegenDelay)
	
	UPROPERTY(BlueprintReadOnly, Category="Attributes", meta=(AllowPrivateAccess="true"))
	FGameplayAttributeData ShieldRegenRate;
	ATTRIBUTE_ACCESSORS(UCharacterBaseAttributeSet, ShieldRegenRate)
};
