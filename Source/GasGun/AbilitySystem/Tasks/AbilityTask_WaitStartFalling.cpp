// Copyright 2025 Dale "Stropheum" Diaz


#include "AbilityTask_WaitStartFalling.h"

#include "GasGun/Guns/Projectiles/FlechetteProjectile.h"
#include "Net/UnrealNetwork.h"


UAbilityTask_WaitStartFalling::UAbilityTask_WaitStartFalling()
{
	bTickingTask = true;
	bSimulatedTask = true;
}

UAbilityTask_WaitStartFalling* UAbilityTask_WaitStartFalling::OnStartFallingEvent(UGameplayAbility* OwningAbility, const FName TaskInstanceName, AFlechetteProjectile* FlechetteProjectile)
{
	UAbilityTask_WaitStartFalling* Task = NewAbilityTask<UAbilityTask_WaitStartFalling>(OwningAbility, TaskInstanceName);
	Task->TrackedProjectile = FlechetteProjectile;
	return Task;
}

void UAbilityTask_WaitStartFalling::TickTask(const float DeltaTime)
{
	Super::TickTask(DeltaTime);

	const FVector CurrentVelocity = TrackedProjectile->GetVelocity();
	if (CurrentVelocity.Z > 0.f)
	{
		return;
	}

	if (StartFallingEventReceived.IsBound())
	{
		StartFallingEventReceived.Broadcast(TrackedProjectile);
	}
}

void UAbilityTask_WaitStartFalling::OnDestroy(const bool AbilityEnded)
{
	bTickingTask = false;
	
	Super::OnDestroy(AbilityEnded);
}

void UAbilityTask_WaitStartFalling::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UAbilityTask_WaitStartFalling, TrackedProjectile);
}
