// Copyright 2025 Dale "Stropheum" Diaz

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "CharacterDamageExecution.generated.h"

/**
 * 
 */
UCLASS()
class GASGUN_API UCharacterDamageExecution : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UCharacterDamageExecution();
	
	virtual void Execute_Implementation(
		const FGameplayEffectCustomExecutionParameters& ExecutionParams,
		FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
