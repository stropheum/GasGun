// Copyright 2025 Dale "Stropheum" Diaz

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GasGunGameMode.generated.h"

UCLASS(minimalapi)
class AGasGunGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AGasGunGameMode();

protected:
	virtual void PostLogin(APlayerController* NewPlayer) override;

	UPROPERTY()
	TArray<TObjectPtr<APlayerController>> PlayerControllerList{};
};



