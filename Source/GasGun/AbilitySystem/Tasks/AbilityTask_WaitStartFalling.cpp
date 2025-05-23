// Copyright 2025 Dale "Stropheum" Diaz


#include "AbilityTask_WaitStartFalling.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "GasGun/Guns/Projectiles/FlechetteProjectile.h"


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
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, FString::Printf(TEXT("Velocity: %s"), *CurrentVelocity.ToString()));
	if (CurrentVelocity.Z > 0.f)
	{
		return;
	}

	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString(TEXT("FALLING DETECTED")));
	if (StartFallingEventReceived.IsBound())
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString(TEXT("FALLING BROADCAST")));
		StartFallingEventReceived.Broadcast(TrackedProjectile);
	}
}

void UAbilityTask_WaitStartFalling::OnDestroy(const bool AbilityEnded)
{
	bTickingTask = false;
	
	Super::OnDestroy(AbilityEnded);
}
