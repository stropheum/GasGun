// Copyright 2025 Dale "Stropheum" Diaz


#include "NpcWanderComponent.h"

#include "AIController.h"
#include "GasGun/Characters/CharacterBase.h"


UNpcWanderComponent::UNpcWanderComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}

void UNpcWanderComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!GetOwner()->HasAuthority())
	{
		return;
	}

	AActor* OwningActor = GetOwner();
	ensureMsgf(OwningActor, TEXT("Owning actor != nullptr"));
	const ACharacterBase* Character = Cast<ACharacterBase>(OwningActor);
	ensureMsgf(Character, TEXT("Character != nullptr"));
	AController* Controller = Character->GetController();
	ensureMsgf(Controller, TEXT("Controller != nullptr"));
	AIController = Cast<AAIController>(Controller);
	ensureMsgf(AIController, TEXT("AIController != nullptr"));
}

void UNpcWanderComponent::TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!GetOwner()->HasAuthority())
	{
		return;
	}

	TimeSinceLastLocationSelection += DeltaTime;
	if (TimeSinceLastLocationSelection >= LocationSelectionInterval)
	{
		if (AIController->IsFollowingAPath())
		{
			AIController->StopMovement();
		}
		else
		{
			if (UPathFollowingComponent* PathFollowingComponent = AIController->GetPathFollowingComponent())
			{
				PathFollowingComponent->OnRequestFinished.AddUObject(this, &UNpcWanderComponent::OnMoveCompleted);
				APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
				check(PlayerController);
				DrawDebugSphere(GetWorld(), PathFollowingComponent->GetCurrentTargetLocation(), 5.0f, 36, FColor::Cyan, true, 0.0f, 0, 1.0f);

				const auto OwnerLocation = GetOwner()->GetActorLocation();
				const auto TargetLocation = PathFollowingComponent->GetCurrentTargetLocation();
				auto DebugString = FString::Printf(TEXT("%s - %s"), *OwnerLocation.ToString(), *TargetLocation.ToString());
				AIController->MoveToActor(PlayerController);
			}
		}

		TimeSinceLastLocationSelection = 0.0f;
	}
}

void UNpcWanderComponent::OnMoveCompleted(FAIRequestID FaiRequestID, const FPathFollowingResult& PathFollowingResult)
{
	
}