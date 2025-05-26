// Copyright 2025 Dale "Stropheum" Diaz

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_WaitStartFalling.generated.h"

class AFlechetteProjectile;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWaitStartFallingDelegate, AFlechetteProjectile*, FlechetteProjectile);

/**
 * 
 */
UCLASS()
class GASGUN_API UAbilityTask_WaitStartFalling : public UAbilityTask
{
	GENERATED_BODY()

public:
	UAbilityTask_WaitStartFalling();
	
	UPROPERTY(BlueprintAssignable)
	FWaitStartFallingDelegate StartFallingEventReceived;

	UFUNCTION(BlueprintCallable, Meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "True"), Category = "Ability Tasks")
	static UAbilityTask_WaitStartFalling* OnStartFallingEvent(UGameplayAbility* OwningAbility, const FName TaskInstanceName, AFlechetteProjectile* FlechetteProjectile);

protected:
	virtual void TickTask(const float DeltaTime) override;
	
	virtual void OnDestroy(bool AbilityEnded) override;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(Replicated)
	TObjectPtr<AFlechetteProjectile> TrackedProjectile{};
};
