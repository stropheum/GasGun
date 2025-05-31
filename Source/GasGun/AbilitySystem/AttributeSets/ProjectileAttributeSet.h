// Copyright 2025 Dale "Stropheum" Diaz

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSetBase.h"
#include "ProjectileAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class GASGUN_API UProjectileAttributeSet : public UAttributeSetBase
{
	GENERATED_BODY()

public:
	static FName GetDamageTagName() { return FName("Damage.Amount"); }

	UPROPERTY(BlueprintReadOnly, Category="Attributes", meta=(AllowPrivateAccess="true"))
	FGameplayAttributeData Damage = 10.f;
	ATTRIBUTE_ACCESSORS(UProjectileAttributeSet, Damage)
};
