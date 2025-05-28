// Copyright 2025 Dale "Stropheum" Diaz

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_WaitForTagOnOwner.generated.h"

/**
 * Delegate provider for reacting to owner tag changes
 */
UCLASS()
class GASGUN_API UAbilityTask_WaitForTagOnOwner : public UAbilityTask
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FGameplayTag QueryTag;

	UPROPERTY()
	TWeakObjectPtr<UAbilitySystemComponent> Asc;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTagDetectedDelegate);
	UPROPERTY(BlueprintAssignable)
	FTagDetectedDelegate OnTagDetected;

	static UAbilityTask_WaitForTagOnOwner* WaitForTagOnOwner(UGameplayAbility* OwningAbility, UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag QueryTag);

protected:
	virtual void Activate() override;
	void OnTagChanged(const FGameplayTag Tag, int32 NewCount);
};
