// Copyright 2025 Dale "Stropheum" Diaz

#pragma once

#include "CoreMinimal.h"
#include "FireGunAbility_Base.h"
#include "FireGunAbility_Projectile.generated.h"

/**
 * 
 */
UCLASS()
class GASGUN_API UFireGunAbility_Projectile : public UFireGunAbility_Base
{
	GENERATED_BODY()

	virtual void Fire() override;
};
