// Copyright 2025 Dale "Stropheum" Diaz

#pragma once

#include "CoreMinimal.h"
#include "ProjectileAttributeSet.h"
#include "KunaiAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class GASGUN_API UKunaiAttributeSet : public UProjectileAttributeSet
{
	GENERATED_BODY()

public:
	UKunaiAttributeSet();
	virtual void PostInitProperties() override;
};
