// Copyright 2025 Dale "Stropheum" Diaz


#include "NpcWanderComponent.h"

#include "AIController.h"
#include "GasGun/Characters/CharacterBase.h"


UNpcWanderComponent::UNpcWanderComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UNpcWanderComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* OwningActor = GetOwner();
	ensureMsgf(OwningActor, TEXT("Owning actor != nullptr"));
	ACharacterBase* Character = Cast<ACharacterBase>(OwningActor);
	ensureMsgf(Character, TEXT("Character != nullptr"));
	AController* Controller = Character->GetController();
	ensureMsgf(Controller, TEXT("Controller != nullptr"));
	AIController = Cast<AAIController>(Controller);
	ensureMsgf(AIController, TEXT("AIController != nullptr"));

	if (UPathFollowingComponent* PathFollowingComponent = AIController->GetPathFollowingComponent())
	{
		PathFollowingComponent->OnRequestFinished.AddUObject(this, &UNpcWanderComponent::OnMoveCompleted);
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		check(PlayerController);
		AIController->MoveToActor(PlayerController);
	}
}

void UNpcWanderComponent::TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

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
				// GEngine->AddOnScreenDebugMessage(-1, -1, FColor::Cyan, PathFollowingComponent->GetStatusDesc());
				DrawDebugSphere(GetWorld(), PathFollowingComponent->GetCurrentTargetLocation(), 5.0f, 36, FColor::Cyan, true, 0.0f, 0, 1.0f);

				auto OwnerLocation = GetOwner()->GetActorLocation();
				auto TargetLocation = PathFollowingComponent->GetCurrentTargetLocation();
				auto DebugString = FString::Printf(TEXT("%s - %s"), *OwnerLocation.ToString(), *TargetLocation.ToString());
				// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, DebugString);
				DrawDebugLine(GetWorld(), OwnerLocation, TargetLocation, FColor::Red);
				AIController->MoveToActor(PlayerController);
			}
		}

		TimeSinceLastLocationSelection = 0.0f;
	}
}

void UNpcWanderComponent::OnMoveCompleted(FAIRequestID FaiRequestID, const FPathFollowingResult& PathFollowingResult)
{
	
}