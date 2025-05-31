// Copyright 2025 Dale "Stropheum" Diaz

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "ProjectileDirectDamageEffect.generated.h"

/**
 * 
 */
UCLASS()
class GASGUN_API UProjectileDirectDamageEffect : public UGameplayEffect
{
	GENERATED_BODY()

public:
	UProjectileDirectDamageEffect();

	virtual void PostInitProperties() override;
};
