// Copyright 2025 Dale "Stropheum" Diaz

#pragma once

#include "CoreMinimal.h"
#include "AITypes.h"
#include "Components/ActorComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "NpcWanderComponent.generated.h"


class AAIController;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GASGUN_API UNpcWanderComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UNpcWanderComponent();
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

	void OnMoveCompleted(FAIRequestID FaiRequestID, const FPathFollowingResult& PathFollowingResult);
	
	UPROPERTY()
	TObjectPtr<AAIController> AIController{};

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="NPC", meta = (AllowPrivateAccess = "true"))
	float LocationSelectionInterval = 5.0f;

	UPROPERTY(BlueprintReadOnly, Category="NPC", meta = (AllowPrivateAccess = "true"))
	float TimeSinceLastLocationSelection = 0.0f;
};
